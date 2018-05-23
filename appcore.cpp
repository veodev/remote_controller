#include "appcore.h"

#include <QTcpServer>
#include <QDebug>
#include <QDataStream>
#include <QSettings>
#include <QFile>
#include <QtEndian>

const int PING_INTERVAL_MS = 300;
const int WATCHDOG_INTERVAL_MS = 1000;

AppCore::AppCore(QObject *parent) : QObject(parent)
  , _tcpSocket(Q_NULLPTR)
  , _km(0)
  , _pk(0)
  , _m(0)
  , _isNotify(true)
  , _isRegistrationOn(false)
  , _direction(UnknownDirection)
  , _viewType(KmPkM)
  , _ipAddress(QSettings().value("IpAddress").toString())
  , _pingTimer(Q_NULLPTR)
{

    _pingTimer = new QTimer(this);
    _pingTimer->setInterval(PING_INTERVAL_MS);
    connect(_pingTimer, &QTimer::timeout, this, &AppCore::onPingTimerTimeout);

    _watchdog = new QTimer(this);
    _watchdog->setInterval(WATCHDOG_INTERVAL_MS);
    connect(_watchdog, &QTimer::timeout, this, &AppCore::onWatchdogTimeout);
}

void AppCore::onSetIpAddress(QString ipAddress)
{
    _ipAddress = ipAddress;    
    onDisconnectingToServer();
    QTimer::singleShot(3000, this, &AppCore::onConnectingToServer);
}

void AppCore::onConnectToServer()
{
    onConnectingToServer();
}

void AppCore::onNotifyThresholdChanged(int threshold)
{
    _notifyThreshold = threshold;
}

void AppCore::onPingTimerTimeout()
{
    QByteArray message;
    message.append(PingRemoteServer);
    sendMessage(message);
}

void AppCore::onWatchdogTimeout()
{
    _watchdog->stop();
    onDisconnectingToServer();
    emit doSoundLostLink();
}


void AppCore::updateState()
{
    updateTrackMarks();
    updateCurrentCoordinate();
    onNextTrackMark();

    if (_direction == ForwardDirection) {
        emit doIncrease();
    }
    else if (_direction == BackwardDirection) {
        emit doDecrease();
    }

    (_isRegistrationOn == true) ? emit doStartRegistration() : emit doStopRegistration();
}

void AppCore::updateTrackMarks()
{
    _trackMarks.setDirection(_direction);
    _trackMarks.setKm(_km);
    _trackMarks.setPk(_pk);
    _trackMarks.setM(_m);
    _trackMarks.updatePost();
    _tmpTrackMarks = _trackMarks;
}

void AppCore::updateMeters()
{
    _trackMarks.setM(_m);
}

void AppCore::updateCurrentCoordinate()
{
    switch(_viewType) {
    case KmPkM:
        emit doCurrentTrackMarks(_trackMarks.getString());
        break;
    case KmCommaM:
        emit doCurrentTrackMarks(_trackMarks.getKmString());
        break;
    case Hectometer:
        emit doCurrentTrackMarks(_trackMarks.getHmString());
        break;
    case MeterOnly:
        emit doCurrentTrackMarks(_trackMarks.getMeterString());
        break;
    }    
}

void AppCore::updateBridgesModel()
{    
    for (QString& item: _bridgesList) {
        emit addItemToBridgesModel(item);
    }
}

void AppCore::updatePlatformsModel()
{
    for (QString& item: _platformsList) {
        emit addItemToPlatformsModel(item);
    }
}

void AppCore::updateMiscModel()
{
    for (QString& item: _miscList) {
        emit addItemToMiscModel(item);
    }
}

void AppCore::readMessageFromBuffer()
{
    Headers header = UnknownHeader;
    while(true) {
        if (_messagesBuffer.size() >= sizeof(qint16)) {
            quint16 size = qFromLittleEndian<quint16>(reinterpret_cast<const uchar*>(_messagesBuffer.left(sizeof(quint16)).data()));
            if (_messagesBuffer.size() >= size) {
                _messagesBuffer.remove(0, sizeof(quint16));
                header = static_cast<Headers>(_messagesBuffer.at(0));                
                _messagesBuffer.remove(0, sizeof(Headers));
                switch(header) {
                case CurrentMeter:
                    _m = qFromLittleEndian<int>(reinterpret_cast<const uchar*>(_messagesBuffer.left(sizeof(int)).data()));
                    _messagesBuffer.remove(0, sizeof(int));                    
                    checkDistance();
                    updateMeters();
                    updateCurrentCoordinate();
                    break;
                case CurrentSpeed: {
                    float speed = qFromLittleEndian<float>(reinterpret_cast<const uchar*>(_messagesBuffer.left(sizeof(float)).data()));
                    _messagesBuffer.remove(0, sizeof(float));
                    emit doCurrentSpeed(speed);
                    break;
                }
                case CurrentTrackMarks:
                    _km = qFromLittleEndian<int>(reinterpret_cast<const uchar*>(_messagesBuffer.left(sizeof(int)).data()));
                    _messagesBuffer.remove(0, sizeof(int));
                    _pk = _messagesBuffer.at(0);
                    _messagesBuffer.remove(0, sizeof(char));
                    _m = qFromLittleEndian<int>(reinterpret_cast<const uchar*>(_messagesBuffer.left(sizeof(int)).data()));
                    _messagesBuffer.remove(0, sizeof(int));
                    updateTrackMarks();
                    updateCurrentCoordinate();
                    onNextTrackMark();
                    _isNotify = true;
                    break;
                case UpdateState:
                    _isRegistrationOn = _messagesBuffer.at(0);
                    _messagesBuffer.remove(0, sizeof(bool));
                    _viewType = static_cast<ViewCoordinate>(_messagesBuffer.at(0));
                    _messagesBuffer.remove(0, sizeof(ViewCoordinate));
                    _direction = qFromLittleEndian<Direction>(reinterpret_cast<const uchar*>(_messagesBuffer.left(sizeof(int)).data()));
                    _messagesBuffer.remove(0, sizeof(Direction));
                    _km = qFromLittleEndian<int>(reinterpret_cast<const uchar*>(_messagesBuffer.left(sizeof(int)).data()));
                    _messagesBuffer.remove(0, sizeof(int));
                    _pk = _messagesBuffer.at(0);
                    _messagesBuffer.remove(0, sizeof(char));
                    _m = qFromLittleEndian<int>(reinterpret_cast<const uchar*>(_messagesBuffer.left(sizeof(int)).data()));
                    _messagesBuffer.remove(0, sizeof(int));
                    updateState();
                    break;
                case ClearMarksLists:
                    _bridgesList.clear();
                    _platformsList.clear();
                    _miscList.clear();
                    emit clearBridgesModel();
                    emit clearPlatformsModel();
                    emit clearMiscModel();
                    break;
                case UpdateMarksLists:
                    updateBridgesModel();
                    updatePlatformsModel();
                    updateMiscModel();
                    break;
                case BridgesItem: {
                    QString bridge = QString::fromUtf8(_messagesBuffer.left(size - sizeof(Headers)));
                    _messagesBuffer.remove(0, size - sizeof(Headers));
                    if (_bridgesList.contains(bridge) == false) {
                        _bridgesList.append(bridge);
                    }                    
                    break;
                }
                case PlatformsItem: {
                    QString platform = QString::fromUtf8(_messagesBuffer.left(size - sizeof(Headers)));
                    _messagesBuffer.remove(0, size - sizeof(Headers));
                    if (_platformsList.contains(platform) == false) {
                        _platformsList.append(platform);
                    }                    
                    break;
                }
                case MiscItem: {
                    QString misc = QString::fromUtf8(_messagesBuffer.left(size - sizeof(Headers)));
                    _messagesBuffer.remove(0, size - sizeof(Headers));
                    if (_miscList.contains(misc) == false) {
                        _miscList.append(misc);
                    }                    
                    break;
                }
                case PingRemoteControl: {
                    _watchdog->start();
                }

                default:
                    break;
                }
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
}

void AppCore::sendMessage(QByteArray &message)
{
    if (_tcpSocket != Q_NULLPTR) {
        quint16 size = static_cast<quint16>(message.size());
        _tcpSocket->write(reinterpret_cast<char*>(&size), sizeof(quint16));
        _tcpSocket->write(message);
        _tcpSocket->flush();
    }
}

void AppCore::checkDistance()
{
    if (_isNotify && _isRegistrationOn) {
        bool nextForwardPostCondition = (_direction == ForwardDirection) && ((_m >= (100 - _notifyThreshold)) && _m < 100);
        bool nextBackwardPostCondition = (_direction == BackwardDirection) && (_m > 0 && (_m <= _notifyThreshold));
        if (nextForwardPostCondition || nextBackwardPostCondition) {
            _isNotify = false;
            emit doNotForget();
        }
    }
}

void AppCore::onPositionUpdate(const QGeoPositionInfo &info)
{        
    float latitude = static_cast<float>(info.coordinate().latitude());
    float longitude = static_cast<float>(info.coordinate().longitude());
    float altitude = static_cast<float>(info.coordinate().altitude());
    float direction = static_cast<float>(info.attribute((QGeoPositionInfo::Direction)));
    float groundSpeed = static_cast<float>(info.attribute((QGeoPositionInfo::GroundSpeed)));
    int year = static_cast<int>(info.timestamp().date().year());
    char month = static_cast<char>(info.timestamp().date().month());
    char day = static_cast<char>(info.timestamp().date().day());
    char hours = static_cast<char>(info.timestamp().time().hour());
    char minutes = static_cast<char>(info.timestamp().time().minute());
    char seconds = static_cast<char>(info.timestamp().time().second());

    QByteArray message;
    message.append(SatellitesInfo);
    message.append(reinterpret_cast<char*>(&latitude), sizeof(float));
    message.append(reinterpret_cast<char*>(&longitude), sizeof(float));
    message.append(reinterpret_cast<char*>(&altitude), sizeof(float));
    message.append(reinterpret_cast<char*>(&direction), sizeof(float));
    message.append(reinterpret_cast<char*>(&groundSpeed), sizeof(float));
    message.append(reinterpret_cast<char*>(&year), sizeof(int));
    message.append(month);
    message.append(day);
    message.append(hours);
    message.append(minutes);
    message.append(seconds);
    sendMessage(message);
}

void AppCore::onSatellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites)
{
    int count = satellites.count();
    (count >= 3) ? emit satellitesFound() : emit satellitesNotFound();

    QByteArray message;
    message.append(SatellitesInUse);
    message.append(reinterpret_cast<char*>(&count), sizeof(int));
    sendMessage(message);
}

void AppCore::onSatellitesError(QGeoSatelliteInfoSource::Error satelliteError)
{
    if (satelliteError == QGeoSatelliteInfoSource::ClosedError) {
        emit satellitesNotFound();
    }
}

void AppCore::startWork()
{
    initGeo();
    qDebug() << "AppCore started!";
}

void AppCore::initGeo()
{
    _geoPosition = QGeoPositionInfoSource::createDefaultSource(this);
    connect(_geoPosition, &QGeoPositionInfoSource::positionUpdated, this , &AppCore::onPositionUpdate);
    _geoPosition->startUpdates();

    _geoSatellite = QGeoSatelliteInfoSource::createDefaultSource(this);
    connect(_geoSatellite, &QGeoSatelliteInfoSource::satellitesInUseUpdated, this, &AppCore::onSatellitesInUseUpdated);
    connect(_geoSatellite, SIGNAL(error(QGeoSatelliteInfoSource::Error)), this, SLOT(onSatellitesError(QGeoSatelliteInfoSource::Error)));
    _geoSatellite->startUpdates();
    qDebug() << "Geo inited!";
}

void AppCore::onNextTrackMark()
{    
    QString nextValue;
    _tmpTrackMarks.next();
    _tmpTrackMarks.getPostKm(0) == _tmpTrackMarks.getPostKm(1) ? nextValue = QString::number(_tmpTrackMarks.getPostPk(0)) + "/" + QString::number(_tmpTrackMarks.getPostPk(1)) + " пк"
                                                               : nextValue = QString::number(_tmpTrackMarks.getPostKm(0)) + "/" + QString::number(_tmpTrackMarks.getPostKm(1)) + " км";
    emit doNextTrackMarks(nextValue);
}

void AppCore::onPrevTrackMark()
{
    QString nextValue;
    _tmpTrackMarks.prev();
    _tmpTrackMarks.getPostKm(0) == _tmpTrackMarks.getPostKm(1) ? nextValue = QString::number(_tmpTrackMarks.getPostPk(0)) + "/" + QString::number(_tmpTrackMarks.getPostPk(1)) + " пк"
                                                               : nextValue = QString::number(_tmpTrackMarks.getPostKm(0)) + "/" + QString::number(_tmpTrackMarks.getPostKm(1)) + " км";
    emit doNextTrackMarks(nextValue);
}

void AppCore::onSetTrackMark()
{    
    int km = _tmpTrackMarks.getKm();
    char pk = static_cast<char>(_tmpTrackMarks.getPk());
    QByteArray message;
    message.append(CurrentTrackMarks);
    message.append(reinterpret_cast<char*>(&km), sizeof(int));
    message.append(pk);
    sendMessage(message);
}

void AppCore::onStartRegistration()
{
    QByteArray message;
    message.append(StartRegistration);
    sendMessage(message);
}

void AppCore::onStopRegistration()
{    
    QByteArray message;
    message.append(StopRegistration);
    sendMessage(message);
}

void AppCore::onStartSwitch()
{    
    QByteArray message;
    message.append(StartSwitch);
    sendMessage(message);
}

void AppCore::onEndSwitch()
{    
    QByteArray message;
    message.append(EndSwitch);
    sendMessage(message);
}

void AppCore::onBridgeSelected(QString name)
{
    int index = _bridgesList.indexOf(name);
    QByteArray message;
    message.append(BridgesItem);
    message.append(reinterpret_cast<char*>(&index), sizeof(int));
    sendMessage(message);
}

void AppCore::onPlatformSelected(QString name)
{    
    int index = _platformsList.indexOf(name);
    QByteArray message;
    message.append(PlatformsItem);
    message.append(reinterpret_cast<char*>(&index), sizeof(int));
    sendMessage(message);
}

void AppCore::onMiscSelected(QString name)
{    
    int index = _miscList.indexOf(name);
    QByteArray message;
    message.append(MiscItem);
    message.append(reinterpret_cast<char*>(&index), sizeof(int));
    sendMessage(message);
}

void AppCore::onConnectingToServer()
{
    if (_tcpSocket == Q_NULLPTR) {
        _tcpSocket = new QTcpSocket(this);        
        connect(_tcpSocket, &QTcpSocket::readyRead, this, &AppCore::onSocketReadyRead, Qt::DirectConnection);
        connect(_tcpSocket, &QTcpSocket::stateChanged, this, &AppCore::onSocketStateChanged);
        _tcpSocket->connectToHost(_ipAddress, 49001, QTcpSocket::ReadWrite);
        if (_pingTimer->isActive() == false) {
            _pingTimer->start();
        }
    }
}

void AppCore::onDisconnectingToServer()
{
    if (_tcpSocket != Q_NULLPTR) {        
        _pingTimer->stop();
        _tcpSocket->disconnectFromHost();
        disconnect(_tcpSocket, &QTcpSocket::readyRead, this, &AppCore::onSocketReadyRead);
        disconnect(_tcpSocket, &QTcpSocket::stateChanged, this, &AppCore::onSocketStateChanged);
        _tcpSocket->deleteLater();
        _tcpSocket = Q_NULLPTR;
    }
}

void AppCore::onSocketReadyRead()
{
    while (_tcpSocket->bytesAvailable()) {
        _messagesBuffer.append(_tcpSocket->readAll());
    }
    readMessageFromBuffer();
}

void AppCore::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        emit doSocketDisconnected();
        onDisconnectingToServer();
        QTimer::singleShot(3000, this, &AppCore::onConnectingToServer);
        break;
    case QAbstractSocket::ConnectingState:
        emit doSocketConnecting();
        break;
    case QAbstractSocket::ConnectedState:
        emit doSocketConnected();
        break;
    default:
        break;
    }
}
