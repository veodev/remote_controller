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

    _geoPosition = QGeoPositionInfoSource::createDefaultSource(this);
    connect(_geoPosition, &QGeoPositionInfoSource::positionUpdated, this , &AppCore::onPositionUpdate);
    _geoPosition->setUpdateInterval(3000);
    _geoPosition->startUpdates();

    _geoSatellite = QGeoSatelliteInfoSource::createDefaultSource(this);
    connect(_geoSatellite, &QGeoSatelliteInfoSource::satellitesInUseUpdated, this, &AppCore::onSatellitesInUseUpdated);
    connect(_geoSatellite, SIGNAL(error(QGeoSatelliteInfoSource::Error)), this, SLOT(onSatellitesError(QGeoSatelliteInfoSource::Error)));
    _geoSatellite->setUpdateInterval(3000);
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
    QDataStream output(_tcpSocket);
    output << StartRegistration;
}

void AppCore::stopRegistration()
{
    QDataStream output(_tcpSocket);
    output << StopRegistration;
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
    QDataStream output(_tcpSocket);
    output << CurrentTrackMarks << _trackMarks.getKm() << _trackMarks.getPk();
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
    QGeoCoordinate coordinate = info.coordinate();
    if (coordinate.type() == QGeoCoordinate::Coordinate2D) {
        qDebug() << "Coordinate: " << coordinate.toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere);
    }

    qreal direction = info.attribute(QGeoPositionInfo::Direction);
    if (::qIsNaN(direction) == false) {
        qDebug() << "Direction: " << QString::number(direction);
    }

    qreal speed = info.attribute(QGeoPositionInfo::GroundSpeed);
    if (::qIsNaN(speed) == false) {
        qDebug() << "Speed: " << QString::number(speed);
    }

    if (info.timestamp().toString() != "") {
        qDebug() << "Time: " << info.timestamp().toString(Qt::LocaleDate);
    }

    QDataStream output(_tcpSocket);
    output << SatellitesInfo << info.coordinate().toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << direction << speed << info.timestamp().toString();
}

void AppCore::onSatellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites)
{
    auto count = satellites.count();
    QDataStream output(_tcpSocket);
    output << SatellitesInUse << count;
    if (count >= 3) {
        emit satellitesFound();
    }
    else {
        emit satellitesNotFound();
    }
    qDebug() << "In use: " << satellites.count();
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
    QDataStream inputData(_tcpSocket);
    int header;
    int direction;
    int viewType;
    inputData >> header;
    switch (static_cast<Headers>(header)) {
    case CurrentMeter:
        inputData >> _m;
        checkDistance();
        updateCurrentCoordinate();
        break;
    case CurrentSpeed:
        double speed;
        inputData >> speed;
        emit doCurrentSpeed(speed);
        break;
    case CurrentTrackMarks:
        inputData >> _km >> _pk >> _m;
        updateState();
        break;
    case UpdateState:
        inputData >> _isRegistrationOn >> viewType >> direction >> _km >> _pk >> _m;
        _direction = static_cast<Direction>(direction);
        _viewType = static_cast<ViewCoordinate>(viewType);
        updateState();
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
