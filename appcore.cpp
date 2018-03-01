#include "appcore.h"

#include <QTcpServer>
#include <QDebug>
#include <QDataStream>


AppCore::AppCore(QObject *parent) : QObject(parent)
  , _tcpSocket(Q_NULLPTR)
  , _km(0)
  , _pk(0)
  , _m(0)
  , _speed(0)
  , _isSoundEnabled(true)
{
    _mediaPlayer = new QMediaPlayer(this);
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

void AppCore::checkDistance()
{
    if (_isSoundEnabled == true && _m >= 85) {
        _isSoundEnabled = false;
        _mediaPlayer->setMedia(QUrl("qrc:/sounds/bell.wav"));
        _mediaPlayer->play();
    }
}

void AppCore::onConnectingToServer()
{
    if (_tcpSocket == Q_NULLPTR) {
        _tcpSocket = new QTcpSocket(this);
        connect(_tcpSocket, &QTcpSocket::readyRead, this, &AppCore::onSocketReadyRead);
        connect(_tcpSocket, &QTcpSocket::stateChanged, this, &AppCore::onSocketStateChanged);
        _tcpSocket->connectToHost(/*QHostAddress::Any*/ "192.168.10.101", 49001, QTcpSocket::ReadWrite);
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
    inputData >> _km >> _pk >> _m >> _speed;
    emit newData(_km, _pk, _m);
    checkDistance();
}

void AppCore::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        emit doSocketDisconnected();
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
    qDebug() << state;
}
