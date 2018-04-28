#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QMediaPlayer>
#include <QTimer>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>

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
    MiscItem
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
    Q_INVOKABLE void marksSelected(QString name);

private:
#ifdef ANDROID
    void keepScreenOn(bool on);
#endif
    void updateState();
    void updateTrackMarks();
    void updateCurrentCoordinate();

    void updateBridgesModel();
    void updatePlatformsModel();
    void updateMiscModel();

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

    void soundStatusChanged();
    void ipAddressChanged();

    void satellitesFound();
    void satellitesNotFound();
    void satellitesCount(int count);

    void clearBridgesModel();
    void clearPlatformsModel();
    void clearMiscModel();

    void addItemToBridgesModel(QString name);
    void addItemToPlatformsModel(QString name);
    void addItemToMiscModel(QString name);

public slots:
    void onConnectingToServer();
    void onDisconnectingToServer();
    void onSocketReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void checkDistance();
    void nextTrackmark();
    void prevTrackmark();
    void setTrackMarks();
    void onPositionUpdate(const QGeoPositionInfo& info);
    void onSatellitesInUseUpdated(const QList<QGeoSatelliteInfo>& satellites);
    void onSatellitesError(QGeoSatelliteInfoSource::Error satelliteError);

private:
    QTcpSocket* _tcpSocket;
    QDataStream _dataStream;
    QMediaPlayer* _mediaPlayer;
    TMRussian _trackMarks;
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
    bool _isFinishReadData;
};

#endif // APPCORE_H
