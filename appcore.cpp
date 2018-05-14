#include "appcore.h"

#include <QTcpServer>
#include <QDebug>
#include <QDataStream>
#include <QSettings>
#include <QFile>
#include <QtEndian>

AppCore::AppCore(QObject *parent) : QObject(parent)
  , _tcpSocket(Q_NULLPTR)
  , _km(0)
  , _pk(0)
  , _m(0)
  , _speed(0)
  , _isRegistrationOn(false)
  , _direction(UnknownDirection)
  , _viewType(KmPkM)
  , _ipAddress(QSettings().value("IpAddress").toString())
  , _currentHeader(UnknownHeader)
  , _currentCount(-1)
  , _currentCountStrings(-1)
  , _isFinishReadData(true)
  , _isReadList(false)
{
}

int AppCore::getKm()
{
    return _km;
}

int AppCore::getPk()
{
    return _pk;
}

int AppCore::getM()
{
    return _m;
}

void AppCore::onSetIpAddress(QString ipAddress)
{
    _ipAddress = ipAddress;    
    onDisconnectingToServer();
    QTimer::singleShot(3000, this, &AppCore::onConnectingToServer);
}

void AppCore::onSetSoundStatus(bool isEnabled)
{
    _isSoundEnabled = isEnabled;        
}

void AppCore::onConnectToServer()
{
    onConnectingToServer();
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

void AppCore::readItem(Headers header, QStringList& list)
{
    if (_tcpSocket->bytesAvailable() >= 4 && _isFinishReadData == true) {
        QByteArray array = _tcpSocket->read(4);
        _currentCount = qFromLittleEndian<int>(reinterpret_cast<const uchar*>(array.data()));
        _isFinishReadData = false;
        _currentData.clear();
    }

    while (_tcpSocket->bytesAvailable()) {
        if (_tcpSocket->bytesAvailable() >= _currentCount) {
            for (int i = 0; i < _currentCount; ++i) {                
                char byte;
                _tcpSocket->read(&byte, 1);
                _currentData.append(byte);
            }
            _isFinishReadData = true;
            _currentCount = -1;
            list.append(QString::fromUtf8(_currentData));            
            --_currentCountStrings;

            if (_currentCountStrings == 0) {
                _currentCountStrings = -1;
                _isReadList = false;
                switch (header) {
                case BridgesItem:
                    emit updateBridgesModel();
                    break;
                case PlatformsItem:
                    emit updatePlatformsModel();
                    break;
                case MiscItem:
                    emit updateMiscModel();
                    break;
                default:
                    break;
                }
            }
            break;
        }

        if (_tcpSocket->bytesAvailable() < _currentCount) {
            int countReadBytes = _tcpSocket->bytesAvailable();
            _currentData.append(_tcpSocket->readAll());
            _currentCount -= countReadBytes;
            break;
        }
    }
}

void AppCore::checkDistance()
{
    if (_mediaPlayer->state() == QMediaPlayer::PlayingState) {
        return;
    }
    if (_isSoundEnabled == true) {
        if ((_direction == ForwardDirection && (_m > 80 && _m < 100)) || (_direction == BackwardDirection && (_m > 0 && _m < 20))) {
            _isSoundEnabled = false;
            _mediaPlayer->setPosition(0);
            _mediaPlayer->play();
            emit doNotForget();
        }
    }
}

void AppCore::onPositionUpdate(const QGeoPositionInfo &info)
{        
    if (_tcpSocket!= Q_NULLPTR) {
        _tcpSocket->flush();
        _dataStream << SatellitesInfo
               << float(info.coordinate().latitude())
               << float(info.coordinate().longitude())
               << float(info.coordinate().altitude())
               << float(info.attribute(QGeoPositionInfo::Direction))
               << float(info.attribute(QGeoPositionInfo::GroundSpeed))
               << info.timestamp();        
        _tcpSocket->flush();
    }
}

void AppCore::onSatellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites)
{
    if (_tcpSocket!= Q_NULLPTR) {
        auto count = satellites.count();
        _dataStream << SatellitesInUse << count;
        _tcpSocket->flush();
        (count >= 3) ? emit satellitesFound() : emit satellitesNotFound();
        emit satellitesCount(count);        
    }
}

void AppCore::onSatellitesError(QGeoSatelliteInfoSource::Error satelliteError)
{
    switch (satelliteError) {
    case QGeoSatelliteInfoSource::AccessError:
        break;
    case QGeoPositionInfoSource::ClosedError:
        emit satellitesNotFound();
        break;
    case QGeoPositionInfoSource::NoError:
        break;
    case QGeoPositionInfoSource::UnknownSourceError:
        break;
    }
    qDebug() << satelliteError;
}

void AppCore::startWork()
{
    qDebug() << "AppCore started!";
    initMedia();
    initGeo();
}

void AppCore::initMedia()
{
    _mediaPlayer = new QMediaPlayer(this);
    _mediaPlayer->setMedia(QUrl("qrc:/sounds/bike_bell.wav"));
    _mediaPlayer->setVolume(100);
    qDebug() << "Media inited!";
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
    _dataStream << CurrentTrackMarks << _tmpTrackMarks.getKm() << _tmpTrackMarks.getPk();
    _tcpSocket->flush();
}

void AppCore::onStartRegistration()
{
    if (_tcpSocket!= Q_NULLPTR) {
        _dataStream << StartRegistration;
        _tcpSocket->flush();
    }
}

void AppCore::onStopRegistration()
{    
    if (_tcpSocket!= Q_NULLPTR) {
        _dataStream << StopRegistration;
        _tcpSocket->flush();
    }
}

void AppCore::onStartSwitch()
{    
    if (_tcpSocket!= Q_NULLPTR) {
        _dataStream << StartSwitch;
        _tcpSocket->flush();
    }
}

void AppCore::onEndSwitch()
{    
    if (_tcpSocket!= Q_NULLPTR) {
        _dataStream << EndSwitch;
        _tcpSocket->flush();
    }
}

void AppCore::onBridgeSelected(QString name)
{
    if (_tcpSocket!= Q_NULLPTR) {
        _dataStream << BridgesItem << _bridgesList.indexOf(name);
        _tcpSocket->flush();
    }
}

void AppCore::onPlatformSelected(QString name)
{    
    if (_tcpSocket!= Q_NULLPTR) {
        _dataStream << PlatformsItem << _platformsList.indexOf(name);
        _tcpSocket->flush();
    }
}

void AppCore::onMiscSelected(QString name)
{    
    if (_tcpSocket!= Q_NULLPTR) {
        _dataStream << MiscItem << _miscList.indexOf(name);
        _tcpSocket->flush();
    }
}

void AppCore::onConnectingToServer()
{
    if (_tcpSocket == Q_NULLPTR) {
        _tcpSocket = new QTcpSocket(this);
        _dataStream.setDevice(_tcpSocket);
        _tcpSocket->setReadBufferSize(32);
        connect(_tcpSocket, &QTcpSocket::readyRead, this, &AppCore::onSocketReadyRead, Qt::DirectConnection);
        connect(_tcpSocket, &QTcpSocket::stateChanged, this, &AppCore::onSocketStateChanged);
        _tcpSocket->connectToHost(_ipAddress, 49001, QTcpSocket::ReadWrite);        
    }
}

void AppCore::onDisconnectingToServer()
{
    if (_tcpSocket != Q_NULLPTR) {
        _tcpSocket->disconnectFromHost();
        disconnect(_tcpSocket, &QTcpSocket::readyRead, this, &AppCore::onSocketReadyRead);
        disconnect(_tcpSocket, &QTcpSocket::stateChanged, this, &AppCore::onSocketStateChanged);
        _tcpSocket->deleteLater();
        _tcpSocket = Q_NULLPTR;        
    }
}

void AppCore::onSocketReadyRead()
{
    int header;
    if (_isFinishReadData) {
        _dataStream >> header;
        _currentHeader = static_cast<Headers>(header);
    }

    switch (_currentHeader) {
    case CurrentMeter:        
        _dataStream >> _m;        
        checkDistance();
        updateMeters();
        updateCurrentCoordinate();
        break;
    case CurrentSpeed:
        double speed;
        _dataStream >> speed;        
        emit doCurrentSpeed(speed);
        break;
    case CurrentTrackMarks: {
        _dataStream >> _km >> _pk >> _m;
        updateTrackMarks();
        updateCurrentCoordinate();        
        onNextTrackMark();
        _isSoundEnabled = true;
        break;
    }
    case UpdateState:
        int viewType;
        int direction;
        _dataStream >> _isRegistrationOn >> viewType >> direction >> _km >> _pk >> _m;        
        _direction = static_cast<Direction>(direction);
        _viewType = static_cast<ViewCoordinate>(viewType);
        updateState();
        break;
    case BridgesList:
        _bridgesList.clear();
        emit clearBridgesModel();
        _dataStream >> _currentCountStrings;        
        _isReadList = (_currentCountStrings > 0);
        break;    
    case PlatformsList:        
        _platformsList.clear();
        emit clearPlatformsModel();
        _dataStream >> _currentCountStrings;        
        _isReadList = (_currentCountStrings > 0);
        break;
    case MiscList:        
        _miscList.clear();
        emit clearMiscModel();
        _dataStream >> _currentCountStrings;        
        _isReadList = (_currentCountStrings > 0);
        break;
    case BridgesItem:        
        readItem(_currentHeader, _bridgesList);
        break;
    case PlatformsItem:        
        readItem(_currentHeader, _platformsList);
        break;
    case MiscItem:        
        readItem(_currentHeader, _miscList);
        break;
    default:
        break;
    }    
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
