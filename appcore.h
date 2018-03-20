#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QTcpSocket>
#include <QMediaPlayer>
#include <QTimer>

#include "tmrussian.h"
#include "enums.h"

enum Headers {
    StartRegistration,
    StopRegistration,
    CurrentMeter,
    CurrentSpeed,
    CurrentTrackMarks,
    Mark,
    UpdateState
};

class AppCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool _isSoundEnabled READ getSoundStatus WRITE setSoundStatus NOTIFY soundStatusChanged)
    Q_PROPERTY(QString _ipAddress READ getIpAddress WRITE setIpAddress NOTIFY ipAddressChanged)
public:
    explicit AppCore(QObject *parent = nullptr);

    int getKm();
    int getPk();
    int getM();

    QString & getIpAddress();
    void setIpAddress(QString ipAddress);

    bool getSoundStatus();
    void setSoundStatus(bool isEnabled);

    Q_INVOKABLE void startRegistration();
    Q_INVOKABLE void stopRegistration();

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
    void doCurrentMeter(int m);
    void doStartRegistration();
    void doStopRegistration();
    void doIncrease();
    void doDecrease();
    void doCurrentSpeed(double speed);
    void doCurrentTrackMarks(int km, int pk, int m);
    void doNextTrackMarks(QString value);

    void soundStatusChanged();
    void ipAddressChanged();

public slots:
    void onConnectingToServer();
    void onDisconnectingToServer();
    void onSocketReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void checkDistance();
    void nextTrackmark();
    void prevTrackmark();
    void setTrackMarks();

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
    Direction _direction;
    QString _ipAddress;
};

#endif // APPCORE_H
