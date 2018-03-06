#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QTcpSocket>
#include <QMediaPlayer>
#include <QTimer>

#include "tmrussian.h"

enum Headers {
    StartRegistration,
    StopRegistration,
    CurrentMeter,
    Mark,
    UpdateState
};

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = nullptr);

    int getKm();
    int getPk();
    int getM();

private:
#ifdef ANDROID
    void keepScreenOn(bool on);
#endif
    void updateState();
    void updateTrackMarks();

signals:
    void doSocketConnected();
    void doSocketDisconnected();
    void doSocketConnecting();
    void doNewData(int km, int pk, int m);
    void doCurrentMeterAndSpeed(int m, int speed);
    void doStartRegistration(int km, int pk, int m);
    void doStopRegistration();
    void doIncrease();
    void doDecrease();

public slots:
    void onConnectingToServer();
    void onDisconnectingToServer();
    void onSocketReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void checkDistance();
    void nextTrackmark();
    void prevTrackmark();
    void readIpAdress();

private:
    QTcpSocket* _tcpSocket;
    QMediaPlayer* _mediaPlayer;
    TMRussian _trackMarks;
    int _km;
    int _pk;
    int _m;
    int _speed;
    bool _isSoundEnabled;
    bool _isRegistrationOn;
    bool _isIncrease;
};

#endif // APPCORE_H
