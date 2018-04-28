#include "appcore.h"

#include <QTcpServer>
#include <QDebug>
#include <QDataStream>
#include <QSettings>
#include <QFile>

#ifdef ANDROID
    #include <QtAndroidExtras>
#endif

AppCore::AppCore(QObject *parent) : QObject(parent)
  , _tcpSocket(Q_NULLPTR)
  , _km(0)
  , _pk(0)
  , _m(0)
  , _speed(0)
  , _isRegistrationOn(false)
  , _direction(UnknownDirection)
  , _viewType(KmPkM)
  , _currentHeader(UnknownHeader)
  , _currentCount(-1)
  , _currentCountStrings(-1)
  , _isFinishReadData(true)
{
#ifdef ANDROID
    keepScreenOn(true);
#endif
    _mediaPlayer = new QMediaPlayer(this);
    _mediaPlayer->setMedia(QUrl("qrc:/sounds/bike_bell.wav"));
    _mediaPlayer->setVolume(100);
    setIpAddress(QSettings().value("IpAddress").toString());
    setSoundStatus(QSettings().value("IsSoundEnable").toBool());
    onConnectingToServer();

    qDebug() << "Available sources count: " << QGeoPositionInfoSource::availableSources().count();
    _geoPosition = QGeoPositionInfoSource::createDefaultSource(this);
    connect(_geoPosition, &QGeoPositionInfoSource::positionUpdated, this , &AppCore::onPositionUpdate);
    _geoPosition->setUpdateInterval(500);
    _geoPosition->startUpdates();

    qDebug() << "Available sources count: " << QGeoSatelliteInfoSource::availableSources().count();
    _geoSatellite = QGeoSatelliteInfoSource::createDefaultSource(this);
    connect(_geoSatellite, &QGeoSatelliteInfoSource::satellitesInUseUpdated, this, &AppCore::onSatellitesInUseUpdated);
    connect(_geoSatellite, SIGNAL(error(QGeoSatelliteInfoSource::Error)), this, SLOT(onSatellitesError(QGeoSatelliteInfoSource::Error)));
    _geoSatellite->setUpdateInterval(500);
    _geoSatellite->startUpdates();
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

QString &AppCore::getIpAddress()
{
    return _ipAddress;
}

bool AppCore::getSoundStatus()
{
    return _isSoundEnabled;
}

void AppCore::setIpAddress(QString ipAddress)
{
    _ipAddress = ipAddress;
    QSettings settings;
    settings.setValue("IpAddress", ipAddress);
    emit ipAddressChanged();
}

void AppCore::setSoundStatus(bool isEnabled)
{
    _isSoundEnabled = isEnabled;
    QSettings settings;
    settings.setValue("IsSoundEnable", isEnabled);
    emit soundStatusChanged();
}

void AppCore::startRegistration()
{    
    _dataStream << StartRegistration;
}

void AppCore::stopRegistration()
{    
    _dataStream << StopRegistration;
}

void AppCore::marksSelected(QString name)
{
    qDebug() << name;
}

void AppCore::updateState()
{
    updateTrackMarks();
    updateCurrentCoordinate();
    if (_direction == ForwardDirection) {
        emit doIncrease();
    }
    else if (_direction == BackwardDirection) {
        emit doDecrease();
    }

    if (_isRegistrationOn == true) {
        emit doStartRegistration();
    }
    else {
        emit doStopRegistration();
    }

    _trackMarks.next();
    QString nextValue;
    if (_trackMarks.getPostKm(0) == _trackMarks.getPostKm(1)) {
        nextValue = QString::number(_trackMarks.getPostPk(0)) + "/" + QString::number(_trackMarks.getPostPk(1)) + " пк";
    }
    else {
        nextValue = QString::number(_trackMarks.getPostKm(0)) + "/" + QString::number(_trackMarks.getPostKm(1)) + " км";
    }
    emit doNextTrackMarks(nextValue);
}

void AppCore::updateTrackMarks()
{
    _trackMarks.setDirection(_direction);
    _trackMarks.setKm(_km);
    _trackMarks.setPk(_pk);
    _trackMarks.setM(_m);
    _trackMarks.updatePost();

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
    for (auto item: _bridgesList) {
        emit addItemToBridgesModel(item);
    }
}

void AppCore::updatePlatformsModel()
{
    for (auto item: _platformsList) {
        emit addItemToPlatformsModel(item);
    }
}

void AppCore::updateMiscModel()
{
    for (auto item: _miscList) {
        emit addItemToMiscModel(item);
    }
}

#ifdef ANDROID
void AppCore::keepScreenOn(bool on)
{
    QtAndroid::runOnAndroidThread([on]{
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if (activity.isValid()) {
        QAndroidJniObject window =
            activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

        if (window.isValid()) {
          const int FLAG_KEEP_SCREEN_ON = 128;
          if (on) {
            window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
          } else {
            window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
          }
        }
      }
      QAndroidJniEnvironment env;
      if (env->ExceptionCheck()) {
        env->ExceptionClear();
      }
    });
}
#endif

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
        }
    }
}

void AppCore::nextTrackmark()
{
    QString nextValue;
    _trackMarks.next();
    _trackMarks.updatePost();
    if (_trackMarks.getPostKm(0) == _trackMarks.getPostKm(1)) {
        nextValue = QString::number(_trackMarks.getPostPk(0)) + "/" + QString::number(_trackMarks.getPostPk(1)) + " пк";
    }
    else {
        nextValue = QString::number(_trackMarks.getPostKm(0)) + "/" + QString::number(_trackMarks.getPostKm(1)) + " км";
    }
    emit doNextTrackMarks(nextValue);
}

void AppCore::prevTrackmark()
{
    QString nextValue;
    _trackMarks.prev();
    _trackMarks.updatePost();
    if (_trackMarks.getPostKm(0) == _trackMarks.getPostKm(1)) {
        nextValue = QString::number(_trackMarks.getPostPk(0)) + "/" + QString::number(_trackMarks.getPostPk(1)) + " пк";
    }
    else {
        nextValue = QString::number(_trackMarks.getPostKm(0)) + "/" + QString::number(_trackMarks.getPostKm(1)) + " км";
    }
    emit doNextTrackMarks(nextValue);
}

void AppCore::setTrackMarks()
{    
    _dataStream << CurrentTrackMarks << _trackMarks.getKm() << _trackMarks.getPk();
    QString nextValue;
    _trackMarks.next();
    if (_trackMarks.getPostKm(0) == _trackMarks.getPostKm(1)) {
        nextValue = QString::number(_trackMarks.getPostPk(0)) + "/" + QString::number(_trackMarks.getPostPk(1)) + " пк";
    }
    else {
        nextValue = QString::number(_trackMarks.getPostKm(0)) + "/" + QString::number(_trackMarks.getPostKm(1)) + " км";
    }
    emit doNextTrackMarks(nextValue);
    _isSoundEnabled = true;
}

void AppCore::onPositionUpdate(const QGeoPositionInfo &info)
{    
    _dataStream << SatellitesInfo
           << float(info.coordinate().latitude())
           << float(info.coordinate().longitude())
           << float(info.coordinate().altitude())
           << float(info.attribute(QGeoPositionInfo::Direction))
           << float(info.attribute(QGeoPositionInfo::GroundSpeed))
           << info.timestamp().date().year()
           << uchar(info.timestamp().date().month())
           << uchar(info.timestamp().date().day())
           << uchar(info.timestamp().time().hour())
           << uchar(info.timestamp().time().minute())
           << uchar(info.timestamp().time().second());
}

void AppCore::onSatellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites)
{
    auto count = satellites.count();    
    _dataStream << SatellitesInUse << count;
    if (count >= 3) {
        emit satellitesFound();
    }
    else {
        emit satellitesNotFound();
    }
    emit satellitesCount(count);
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
    int direction;
    int viewType;
    QString item;
    int count;    
    _dataStream >> header;
    _currentHeader = static_cast<Headers>(header);

    switch (header) {
    case CurrentMeter:
        _dataStream >> _m;
        checkDistance();
        updateCurrentCoordinate();
        break;
    case CurrentSpeed:
        double speed;
        _dataStream >> speed;
        emit doCurrentSpeed(speed);
        break;
    case CurrentTrackMarks:
        _dataStream >> _km >> _pk >> _m;
        updateState();
        break;
    case UpdateState:
        _dataStream >> _isRegistrationOn >> viewType >> direction >> _km >> _pk >> _m;
        _direction = static_cast<Direction>(direction);
        _viewType = static_cast<ViewCoordinate>(viewType);
        updateState();
        break;
    case BridgesList:
        _bridgesList.clear();
        _dataStream >> _currentCountStrings;
        qDebug() << "_currentCountStrings = " << _currentCountStrings;
        emit clearBridgesModel();
        break;
    case BridgesItem:
        _dataStream >> _currentCount;
        while (_tcpSocket->bytesAvailable()) {
            QChar ch;
            _dataStream >> ch;
            _currentString.append(ch);
            --_currentCount;
            if (_currentCount == 0) {
                _bridgesList.append(_currentString);
                _currentString.clear();
                --_currentCountStrings;
                _currentCount = -1;
                _isFinishReadData = true;
                _tcpSocket->readAll();
                qDebug() << "Bytes available: " << _tcpSocket->bytesAvailable();
                break;
            }
        }

        if (_currentCount > 0) {
            _isFinishReadData = false;
        }

        if (_currentCountStrings == 0) {
            emit updateBridgesModel();
            _currentCountStrings = -1;
        }
        break;
    case PlatformsList:
        _tcpSocket->readAll();
        break;
    case MiscList:
        _tcpSocket->readAll();
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
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::BoundState:
    case QAbstractSocket::ClosingState:
    case QAbstractSocket::ListeningState:
        break;
    }
}
