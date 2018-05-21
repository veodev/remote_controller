#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QMediaPlayer>
#include <QTimer>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>
#include <QTimer>

#include "tmrussian.h"

enum Headers: char
{
    UnknownHeader,
    StartRegistration,
    StopRegistration,
    CurrentMeter,
    CurrentSpeed,
    CurrentTrackMarks,    
    UpdateState,
    ClearMarksLists,
    UpdateMarksLists,
    SatellitesInfo,
    SatellitesInUse,    
    BridgesItem,
    PlatformsItem,
    MiscItem,
    Ping,
    StartSwitch,
    EndSwitch
};

class AppCore : public QObject
{
    Q_OBJECT    
public:
    explicit AppCore(QObject *parent = nullptr);

private:
    void updateState();    
    void updateTrackMarks();
    void updateMeters();
    void updateCurrentCoordinate();

    void updateBridgesModel();
    void updatePlatformsModel();
    void updateMiscModel();    
    void readMessageFromBuffer();    
    void sendMessage(QByteArray& message);
    void checkDistance();

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
    void doCurrentSpeed(float speed);
    void doNextTrackMarks(QString value);
    void doCurrentTrackMarks(QString value);

    void satellitesFound();
    void satellitesNotFound();
    void satellitesCount(int count);

    void clearBridgesModel();
    void clearPlatformsModel();
    void clearMiscModel();

    void addItemToBridgesModel(QString name);
    void addItemToPlatformsModel(QString name);
    void addItemToMiscModel(QString name);

    void doNotForget();

public slots:
    void onConnectingToServer();
    void onDisconnectingToServer();
    void onSocketReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void onPositionUpdate(const QGeoPositionInfo& info);
    void onSatellitesInUseUpdated(const QList<QGeoSatelliteInfo>& satellites);
    void onSatellitesError(QGeoSatelliteInfoSource::Error satelliteError);

    void startWork();    
    void initGeo();
    void onNextTrackMark();
    void onPrevTrackMark();
    void onSetTrackMark();
    void onStartRegistration();
    void onStopRegistration();
    void onStartSwitch();
    void onEndSwitch();
    void onBridgeSelected(QString name);
    void onPlatformSelected(QString name);
    void onMiscSelected(QString name);
    void onSetIpAddress(QString ipAddress);    
    void onConnectToServer();
    void onNotifyThresholdChanged(int threshold);

private slots:
    void onPingTimerTimeout();

private:
    QTcpSocket* _tcpSocket;        
    TMRussian _trackMarks;
    TMRussian _tmpTrackMarks;
    int _km;
    int _pk;
    int _m;
    int _notifyThreshold;
    bool _isNotify;
    bool _isRegistrationOn;
    Direction _direction;
    ViewCoordinate _viewType;
    QString _ipAddress;
    QGeoPositionInfoSource* _geoPosition;
    QGeoSatelliteInfoSource* _geoSatellite;

    QStringList _bridgesList;
    QStringList _platformsList;
    QStringList _miscList;
    QByteArray _messagesBuffer;
    bool _isPingRemoteServer;
    QTimer* _pingTimer;
};

#endif // APPCORE_H
