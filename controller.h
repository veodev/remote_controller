#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class QThread;
class AppCore;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    Q_INVOKABLE void nextTrackMark();
    Q_INVOKABLE void prevTrackMark();
    Q_INVOKABLE void setTrackMark();
    Q_INVOKABLE void startRegistration();
    Q_INVOKABLE void stopRegistration();
    Q_INVOKABLE void startSwitch();
    Q_INVOKABLE void endSwitch();
    Q_INVOKABLE void bridgeSelected(QString name);
    Q_INVOKABLE void platformSelected(QString name);
    Q_INVOKABLE void miscSelected(QString name);

    Q_PROPERTY(bool _isSoundEnabled READ getSoundStatus WRITE setSoundStatus NOTIFY doSoundStatusChanged)
    bool getSoundStatus();
    void setSoundStatus(bool isEnabled);

    Q_PROPERTY(QString _ipAddress READ getIpAddress WRITE setIpAddress NOTIFY doIpAddressChanged)
    QString & getIpAddress();
    void setIpAddress(QString ipAddress);

private:
#ifdef ANDROID
    void keepScreenOn(bool on);
#endif

signals:
    void doSocketConnected();
    void doSocketDisconnected();
    void doStartRegistration();
    void doStopRegistration();
    void doIncrease();
    void doDecrease();
    void doNextTrackMark();
    void doPrevTrackMark();
    void doNextTrackMarks(QString value);
    void doSetTrackMark();
    void doCurrentTrackMark(QString value);
    void doCurrentSpeed(double speed);
    void doAddItemToBridgesModel(QString name);
    void doAddItemToPlatformsModel(QString name);
    void doAddItemToMiscModel(QString name);
    void doBridgeSelected(QString name);
    void doPlatformSelected(QString name);
    void doMiscSelected(QString name);
    void doStartSwitch();
    void doEndSwitch();
    void doClearBridgesModel();
    void doClearPlatformsModel();
    void doClearMiscModel();
    void doSatellitesFound();
    void doSatellitesNotFound();
    void doSatellitesCount(int count);
    void doCurrentMeter(int m);
    void doSoundStatusChanged();
    void doIpAddressChanged();

public slots:

private slots:
    void onAppCoreConnected();
    void onAppCoreDisconnected();
    void onAppCoreIncrease();
    void onAppCoreDecrease();
    void onAppCoreStartRegistration();
    void onAppCoreStopRegistration();
    void onAppCoreNextTrackMark(QString nextValue);
    void onAppCoreClearBridgesModel();
    void onAppCoreClearPlatformsModel();
    void onAppCoreClearMiscModel();
    void onAppCoreAddItemToBridgesModel(QString item);
    void onAppCoreAddItemToPlatformsModel(QString item);
    void onAppCoreAddItemToMiscModel(QString item);
    void onSatellitesFound();
    void onSatellitesNotFound();
    void onSatellitesCount(int count);
    void onAppCoreCurrentMeter(int m);
    void onAppCoreCurrentTrackMark(QString value);
    void onAppCoreCurrentSpeed(double speed);


private:
    QThread* _appCoreThread;
    AppCore* _appCore;

    bool _isSoundEnabled;
    QString _ipAddress;
};

#endif // CONTROLLER_H
