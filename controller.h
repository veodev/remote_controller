#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QMediaPlayer>

class QThread;
class AppCore;
class Audio;

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
    Q_INVOKABLE void bridgeSelected(int index);
    Q_INVOKABLE void platformSelected(int index);
    Q_INVOKABLE void miscSelected(int index);
    Q_INVOKABLE void setNotifyThreshold(int threshold);

    Q_PROPERTY(QString _ipAddress READ getIpAddress WRITE setIpAddress NOTIFY doIpAddressChanged)
    QString & getIpAddress();
    void setIpAddress(QString ipAddress);

    Q_PROPERTY(bool _isNotifyEnabled READ getNotifyStatus WRITE setNotifyStatus NOTIFY doNotifyStatusChanged)
    void setNotifyStatus(bool isEnabled);
    bool getNotifyStatus();

    Q_PROPERTY(bool _isNotifySoundEnabled READ getNotifySoundStatus WRITE setNotifySoundStatus NOTIFY doNotifySoundStatusChanged)
    void setNotifySoundStatus(bool isEnabled);
    bool getNotifySoundStatus();

    Q_PROPERTY(bool _isNotifyGraphicsEnabled READ getNotifyGraphicsStatus WRITE setNotifyGraphicsStatus NOTIFY doNotifyGraphicsStatusChanged)
    void setNotifyGraphicsStatus(bool isEnabled);
    bool getNotifyGraphicsStatus();

    Q_PROPERTY(int _notifyThresholdIndex READ getNotifyThresholdIndex WRITE setNotifyThresholdIndex NOTIFY doNotifyThresholdIndexChanged)
    void setNotifyThresholdIndex(int index);
    int getNotifyThresholdIndex();

private:
#ifdef ANDROID
    void keepScreenOn(bool on);
#endif
    void checkItemLength(QString& item);

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
    void doCurrentSpeed(float speed);
    void doAddItemToBridgesModel(QString name, int index);
    void doAddItemToPlatformsModel(QString name, int index);
    void doAddItemToMiscModel(QString name, int index);
    void doBridgeSelected(int index);
    void doPlatformSelected(int index);
    void doMiscSelected(int index);
    void doStartSwitch();
    void doEndSwitch();
    void doClearBridgesModel();
    void doClearPlatformsModel();
    void doClearMiscModel();
    void doSatellitesFound();
    void doSatellitesNotFound();
    void doSatellitesCount(int count);
    void doCurrentMeter(int m);
    void doIpAddressChanged();
    void doSetIpAddress(QString ipAddress);
    void doConnectToServer();

    void doNotForgetSoundNotify();
    void doNotForgetGraphicsNotify();
    void doNotifyStatusChanged();
    void doNotifySoundStatusChanged();
    void doNotifyGraphicsStatusChanged();
    void doNotifyThresholdIndexChanged();
    void doSetNotifyThresholdChanged(int threshold);
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
    void onAppCoreAddItemToBridgesModel(QString item, int index);
    void onAppCoreAddItemToPlatformsModel(QString item,  int index);
    void onAppCoreAddItemToMiscModel(QString item, int index);
    void onSatellitesFound();
    void onSatellitesNotFound();
    void onSatellitesCount(int count);
    void onAppCoreCurrentMeter(int m);
    void onAppCoreCurrentTrackMark(QString value);
    void onAppCoreCurrentSpeed(float speed);
    void onDoNotForget();


private:
    QThread* _appCoreThread;
    QThread* _audioThread;

    AppCore* _appCore;
    Audio* _audio;

    QString _ipAddress;
    bool _isNotifyEnabled;
    bool _isNotifySoundEnabled;
    bool _isNotifyGraphicsEnabled;
    int _notifyThresholdIndex;
};

#endif // CONTROLLER_H
