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

enum Headers {
    UnknownHeader = -1,
    StartRegistration = 1,
    StopRegistration,
    CurrentMeter,
    CurrentSpeed,
    CurrentTrackMarks,
    Mark,
    UpdateState,
    SatellitesInfo,
    SatellitesInUse,
    BridgesList,
    PlatformsList,
    MiscList,
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

    int getKm();
    int getPk();
    int getM();

private:
    void updateState();    
    void updateTrackMarks();
    void updateMeters();
    void updateCurrentCoordinate();

    void updateBridgesModel();
    void updatePlatformsModel();
    void updateMiscModel();
    void readItem(Headers header, QStringList& list);

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
    void checkDistance();    
    void onPositionUpdate(const QGeoPositionInfo& info);
    void onSatellitesInUseUpdated(const QList<QGeoSatelliteInfo>& satellites);
    void onSatellitesError(QGeoSatelliteInfoSource::Error satelliteError);

    void startWork();
    void initMedia();
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
    void onSetSoundStatus(bool isEnabled);
    void onConnectToServer();

private:
    QTcpSocket* _tcpSocket;
    QDataStream _dataStream;
    QMediaPlayer* _mediaPlayer;
    TMRussian _trackMarks;
    TMRussian _tmpTrackMarks;
    int _km;
    int _pk;
    int _m;
    int _speed;
    bool _isSoundEnabled;
    bool _isRegistrationOn;
    Direction _direction;
    ViewCoordinate _viewType;
    QString _ipAddress;
    QGeoPositionInfoSource* _geoPosition;
    QGeoSatelliteInfoSource* _geoSatellite;

    QStringList _bridgesList;
    QStringList _platformsList;
    QStringList _miscList;

    Headers _currentHeader;
    int _currentCount;
    int _currentCountStrings;
    QString _currentString;
    QByteArray _currentData;
    bool _isFinishReadData;
    bool _isReadList;    
};

#endif // APPCORE_H
