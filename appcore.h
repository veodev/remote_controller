#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QTcpSocket>
#include <QMediaPlayer>

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = nullptr);

    int getKm();
    int getPk();
    int getM();

private:

signals:
    void doSocketConnected();
    void doSocketDisconnected();
    void doSocketConnecting();
    void newData(int km, int pk, int m);

public slots:
    void onConnectingToServer();
    void onDisconnectingToServer();
    void onSocketReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState state);

    void checkDistance();

private:
    QTcpSocket* _tcpSocket;
    int _km;
    int _pk;
    int _m;
    int _speed;
    QMediaPlayer* _mediaPlayer;
    bool _isSoundEnabled;
};

#endif // APPCORE_H
