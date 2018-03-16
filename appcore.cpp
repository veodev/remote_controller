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
  , _isIncrease(false)
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
    qDebug() << "Start registration";
}

void AppCore::stopRegistration()
{
    QDataStream output(_tcpSocket);
    output << StopRegistration;
    qDebug() << "Stop registration";
}

void AppCore::updateState()
{
    if (_isRegistrationOn) {
        updateTrackMarks();
        emit doStartRegistration(_km, _pk, _m);
    }
    else {
        emit doStopRegistration();
    }
}

void AppCore::updateTrackMarks()
{
    _trackMarks.setKm(_km);
    _trackMarks.setPk(_pk);
    _trackMarks.setM(_m);
    _trackMarks.updatePost();

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
        _isSoundEnabled = false;
        _mediaPlayer->setPosition(0);
        _mediaPlayer->play();
    }
}

void AppCore::nextTrackmark()
{
    _trackMarks.next();
    _trackMarks.updatePost();
    emit doNewData(_trackMarks.getKm(), _trackMarks.getPk(), _trackMarks.getM());
}

void AppCore::prevTrackmark()
{
    _trackMarks.prev();
    _trackMarks.updatePost();
    emit doNewData(_trackMarks.getKm(), _trackMarks.getPk(), _trackMarks.getM());
}

void AppCore::onConnectingToServer()
{
    if (_tcpSocket == Q_NULLPTR) {
        _tcpSocket = new QTcpSocket(this);
        _tcpSocket->setReadBufferSize(16);
        connect(_tcpSocket, &QTcpSocket::readyRead, this, &AppCore::onSocketReadyRead, Qt::DirectConnection);
        connect(_tcpSocket, &QTcpSocket::stateChanged, this, &AppCore::onSocketStateChanged);
        _tcpSocket->connectToHost(_ipAddress, 49001, QTcpSocket::ReadWrite /*| QTcpSocket::Unbuffered*/);
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
    inputData >> header;
    switch (static_cast<Headers>(header)) {
    case StartRegistration:
        _isRegistrationOn = true;
        inputData >> _isIncrease >> _km >> _pk >> _m;
        updateTrackMarks();
        emit doStartRegistration(_km, _pk, _m);
        if (_isIncrease) {
            emit doIncrease();
        }
        else {
            emit doDecrease();
        }
        break;
    case StopRegistration:
        _isRegistrationOn = false;
        emit doStopRegistration();
        break;
    case CurrentMeter:
        inputData >> _m >> _speed;
        checkDistance();
//        if (_isRegistrationOn) {
            emit doCurrentMeterAndSpeed(_m, _speed);
//        }
        break;
    case Mark:
        break;
    case UpdateState:
        inputData >> _isRegistrationOn >> _isIncrease >> _km >> _pk >> _m >> _speed;
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
