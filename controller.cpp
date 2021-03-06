#include "controller.h"
#include "appcore.h"
#include "audio.h"

#include <QThread>
#include <QMetaType>
#include <QSettings>

#ifdef ANDROID
    #include <QtAndroidExtras>
#endif

const QString AVICON31_IP_ADDRESS = "192.168.100.1";
const int MAX_MARKS_ITEM_TEXT_LENGTH = 30;

Controller::Controller(QObject *parent) : QObject(parent)
  , _appCoreThread(new QThread(this))
  , _audioThread(new QThread(this))
  , _appCore(nullptr)
  , _audio(nullptr)  
{
#ifdef ANDROID
    keepScreenOn(true);
#endif

    _appCoreThread->setObjectName("appCoreThread");
    _appCore = new AppCore();
    connect(_appCoreThread, &QThread::started, _appCore, &AppCore::startWork);
    connect(_appCore, &AppCore::doSocketConnected, this, &Controller::onAppCoreConnected);
    connect(_appCore, &AppCore::doSocketDisconnected, this, &Controller::onAppCoreDisconnected);
    connect(_appCore, &AppCore::doIncrease, this, &Controller::onAppCoreIncrease);
    connect(_appCore, &AppCore::doDecrease, this, &Controller::onAppCoreDecrease);
    connect(_appCore, &AppCore::doStartRegistration, this, &Controller::onAppCoreStartRegistration);
    connect(_appCore, &AppCore::doStopRegistration, this, &Controller::onAppCoreStopRegistration);
    connect(_appCore, &AppCore::doNextTrackMarks, this, &Controller::onAppCoreNextTrackMark);
    connect(_appCore, &AppCore::clearBridgesModel, this, &Controller::onAppCoreClearBridgesModel);
    connect(_appCore, &AppCore::clearPlatformsModel, this, &Controller::onAppCoreClearPlatformsModel);
    connect(_appCore, &AppCore::clearMiscModel, this, &Controller::onAppCoreClearMiscModel);
    connect(_appCore, &AppCore::addItemToBridgesModel, this, &Controller::onAppCoreAddItemToBridgesModel);
    connect(_appCore, &AppCore::addItemToPlatformsModel, this, &Controller::onAppCoreAddItemToPlatformsModel);
    connect(_appCore, &AppCore::addItemToMiscModel, this, &Controller::onAppCoreAddItemToMiscModel);
    connect(_appCore, &AppCore::satellitesFound, this, &Controller::onSatellitesFound);
    connect(_appCore, &AppCore::satellitesNotFound, this, &Controller::onSatellitesNotFound);
    connect(_appCore, &AppCore::satellitesCount, this, &Controller::onSatellitesCount);
    connect(_appCore, &AppCore::doCurrentMeter, this, &Controller::onAppCoreCurrentMeter);
    connect(_appCore, &AppCore::doCurrentTrackMarks, this, &Controller::onAppCoreCurrentTrackMark);
    connect(_appCore, &AppCore::doCurrentSpeed, this, &Controller::onAppCoreCurrentSpeed);
    connect(_appCore, &AppCore::doNotForget, this, &Controller::onDoNotForget);

    connect(this, &Controller::doSetIpAddress, _appCore, &AppCore::onSetIpAddress);
    connect(this, &Controller::doConnectToServer, _appCore, &AppCore::onConnectToServer);

    connect(this, &Controller::doNextTrackMark, _appCore, &AppCore::onNextTrackMark);
    connect(this, &Controller::doPrevTrackMark, _appCore, &AppCore::onPrevTrackMark);
    connect(this, &Controller::doSetTrackMark, _appCore, &AppCore::onSetTrackMark);
    connect(this, &Controller::doStartRegistration, _appCore, &AppCore::onStartRegistration);
    connect(this, &Controller::doStopRegistration, _appCore, &AppCore::onStopRegistration);
    connect(this, &Controller::doStartSwitch, _appCore, &AppCore::onStartSwitch);
    connect(this, &Controller::doEndSwitch, _appCore, &AppCore::onEndSwitch);
    connect(this, &Controller::doBridgeSelected, _appCore, &AppCore::onBridgeSelected);
    connect(this, &Controller::doPlatformSelected, _appCore, &AppCore::onPlatformSelected);
    connect(this, &Controller::doMiscSelected, _appCore, &AppCore::onMiscSelected);
    connect(this, &Controller::doSetNotifyThresholdChanged, _appCore, &AppCore::onNotifyThresholdChanged);

    _appCore->moveToThread(_appCoreThread);
    _appCoreThread->start();

    setIpAddress(QSettings().value("IpAddress", AVICON31_IP_ADDRESS).toString());
    setNotifyStatus(QSettings().value("NotifyStatus", true).toBool());
    setNotifySoundStatus(QSettings().value("NotifySoundStatus", true).toBool());
    setNotifyGraphicsStatus(QSettings().value("NotifyGraphicsStatus", true).toBool());
    setNotifyThresholdIndex(QSettings().value("NotifyThresholdIndex", 9).toInt());

    _audioThread->setObjectName("mediaThread");
    _audio = new Audio();
    connect(_audioThread, &QThread::started, _audio, &Audio::initMedia);
    connect(this, &Controller::doNotForgetSoundNotify, _audio, &Audio::playSound);
    connect(_appCore, &AppCore::doSoundLostLink, _audio, &Audio::playDisconnected);
    _audio->moveToThread(_audioThread);
    _audioThread->start();    
}

Controller::~Controller()
{
    disconnect(_appCore, &AppCore::doSocketConnected, this, &Controller::onAppCoreConnected);
    disconnect(_appCore, &AppCore::doSocketDisconnected, this, &Controller::onAppCoreDisconnected);
    disconnect(_appCore, &AppCore::doIncrease, this, &Controller::onAppCoreIncrease);
    disconnect(_appCore, &AppCore::doDecrease, this, &Controller::onAppCoreDecrease);
    disconnect(_appCore, &AppCore::doStartRegistration, this, &Controller::onAppCoreStartRegistration);
    disconnect(_appCore, &AppCore::doStopRegistration, this, &Controller::onAppCoreStopRegistration);
    disconnect(_appCore, &AppCore::doNextTrackMarks, this, &Controller::onAppCoreNextTrackMark);
    disconnect(_appCore, &AppCore::clearBridgesModel, this, &Controller::onAppCoreClearBridgesModel);
    disconnect(_appCore, &AppCore::clearPlatformsModel, this, &Controller::onAppCoreClearPlatformsModel);
    disconnect(_appCore, &AppCore::clearMiscModel, this, &Controller::onAppCoreClearMiscModel);
    disconnect(_appCore, &AppCore::addItemToBridgesModel, this, &Controller::onAppCoreAddItemToBridgesModel);
    disconnect(_appCore, &AppCore::addItemToPlatformsModel, this, &Controller::onAppCoreAddItemToPlatformsModel);
    disconnect(_appCore, &AppCore::addItemToMiscModel, this, &Controller::onAppCoreAddItemToMiscModel);
    disconnect(_appCore, &AppCore::satellitesFound, this, &Controller::onSatellitesFound);
    disconnect(_appCore, &AppCore::satellitesNotFound, this, &Controller::onSatellitesNotFound);
    disconnect(_appCore, &AppCore::satellitesCount, this, &Controller::onSatellitesCount);
    disconnect(_appCore, &AppCore::doCurrentMeter, this, &Controller::onAppCoreCurrentMeter);
    disconnect(_appCore, &AppCore::doCurrentTrackMarks, this, &Controller::onAppCoreCurrentTrackMark);
    disconnect(_appCore, &AppCore::doCurrentSpeed, this, &Controller::onAppCoreCurrentSpeed);

    disconnect(this, &Controller::doNextTrackMark, _appCore, &AppCore::onNextTrackMark);
    disconnect(this, &Controller::doPrevTrackMark, _appCore, &AppCore::onPrevTrackMark);
    disconnect(this, &Controller::doSetTrackMark, _appCore, &AppCore::onSetTrackMark);
    disconnect(this, &Controller::doStartRegistration, _appCore, &AppCore::onStartRegistration);
    disconnect(this, &Controller::doStopRegistration, _appCore, &AppCore::onStopRegistration);
    disconnect(this, &Controller::doStartSwitch, _appCore, &AppCore::onStartSwitch);
    disconnect(this, &Controller::doEndSwitch, _appCore, &AppCore::onEndSwitch);
    disconnect(this, &Controller::doBridgeSelected, _appCore, &AppCore::onBridgeSelected);
    disconnect(this, &Controller::doPlatformSelected, _appCore, &AppCore::onPlatformSelected);
    disconnect(this, &Controller::doMiscSelected, _appCore, &AppCore::onMiscSelected);

    _appCoreThread->quit();
    _appCoreThread->wait(3000);
    if (_appCoreThread->isRunning()) {
        _appCoreThread->terminate();
        _appCoreThread->wait(3000);
    }
    else {
        qDebug() << "AppCoreThread finished!";
    }


    disconnect(_audioThread, &QThread::started, _audio, &Audio::initMedia);
    disconnect(_appCore, &AppCore::doNotForget, _audio, &Audio::playSound);
    _audioThread->quit();
    _audioThread->wait(3000);
    if (_audioThread->isRunning()) {
        _audioThread->terminate();
        _audioThread->wait(3000);
    }
    else {
        qDebug() << "MediaThread finished!";
    }

}

void Controller::nextTrackMark()
{
    emit doNextTrackMark();    
}

void Controller::prevTrackMark()
{
    emit doPrevTrackMark();    
}

void Controller::setTrackMark()
{
    emit doSetTrackMark();    
}

void Controller::startRegistration()
{
    emit doStartRegistration();
}

void Controller::stopRegistration()
{
    emit doStopRegistration();
}

void Controller::startSwitch()
{
    emit doStartSwitch();
}

void Controller::endSwitch()
{
    emit doEndSwitch();
}

void Controller::bridgeSelected(int index)
{    
    qDebug() << index;
    emit doBridgeSelected(index);
}

void Controller::platformSelected(int index)
{
    qDebug() << index;
    emit doPlatformSelected(index);
}

void Controller::miscSelected(int index)
{    
    qDebug() << index;
    emit doMiscSelected(index);
}

void Controller::setNotifyThreshold(int threshold)
{    
    emit doSetNotifyThresholdChanged(threshold);
}

QString &Controller::getIpAddress()
{
    return _ipAddress;
}

void Controller::setIpAddress(QString ipAddress)
{
    _ipAddress = ipAddress;
    QSettings settings;
    settings.setValue("IpAddress", ipAddress);
    emit doIpAddressChanged();
    emit doSetIpAddress(_ipAddress);
}

void Controller::setNotifyStatus(bool isEnabled)
{
    _isNotifyEnabled = isEnabled;
    QSettings settings;
    settings.setValue("NotifyStatus", isEnabled);
    emit doNotifyStatusChanged();
}

bool Controller::getNotifyStatus()
{
    return _isNotifyEnabled;
}

void Controller::setNotifySoundStatus(bool isEnabled)
{
    _isNotifySoundEnabled = isEnabled;
    QSettings settings;
    settings.setValue("NotifySoundStatus", isEnabled);
    emit doNotifySoundStatusChanged();
}

bool Controller::getNotifySoundStatus()
{
    return _isNotifySoundEnabled;
}

void Controller::setNotifyGraphicsStatus(bool isEnabled)
{
    _isNotifyGraphicsEnabled = isEnabled;
    QSettings settings;
    settings.setValue("NotifyGraphicsStatus", isEnabled);
    emit doNotifyGraphicsStatusChanged();
}

bool Controller::getNotifyGraphicsStatus()
{
    return _isNotifyGraphicsEnabled;
}

void Controller::setNotifyThresholdIndex(int index)
{
    _notifyThresholdIndex = index;
    QSettings settings;
    settings.setValue("NotifyThresholdIndex", index);
    emit doNotifyThresholdIndexChanged();
}

int Controller::getNotifyThresholdIndex()
{
    return _notifyThresholdIndex;
}

void Controller::onAppCoreConnected()
{
    emit doSocketConnected();    
}

void Controller::onAppCoreDisconnected()
{
    emit doSocketDisconnected();    
}

void Controller::onAppCoreIncrease()
{
    emit doIncrease();    
}

void Controller::onAppCoreDecrease()
{
    emit doDecrease();
}

void Controller::onAppCoreStartRegistration()
{
    emit doStartRegistration();
}

void Controller::onAppCoreStopRegistration()
{
    emit doStopRegistration();
}

void Controller::onAppCoreNextTrackMark(QString nextValue)
{    
    emit doNextTrackMarks(nextValue);
}

void Controller::onAppCoreClearBridgesModel()
{
    emit doClearBridgesModel();
}

void Controller::onAppCoreClearPlatformsModel()
{
    emit doClearPlatformsModel();
}

void Controller::onAppCoreClearMiscModel()
{
    emit doClearMiscModel();
}

void Controller::onAppCoreAddItemToBridgesModel(QString item,  int index)
{    
    checkItemLength(item);
    emit doAddItemToBridgesModel(item, index);
}

void Controller::onAppCoreAddItemToPlatformsModel(QString item, int index)
{
    checkItemLength(item);
    emit doAddItemToPlatformsModel(item, index);
}

void Controller::onAppCoreAddItemToMiscModel(QString item, int index)
{
    checkItemLength(item);
    emit doAddItemToMiscModel(item, index);
}

void Controller::onSatellitesFound()
{
    emit doSatellitesFound();
}

void Controller::onSatellitesNotFound()
{
    emit doSatellitesNotFound();
}

void Controller::onSatellitesCount(int count)
{
    emit doSatellitesCount(count);
}

void Controller::onAppCoreCurrentMeter(int m)
{
    emit doCurrentMeter(m);
}

void Controller::onAppCoreCurrentTrackMark(QString value)
{
    emit doCurrentTrackMark(value);
}

void Controller::onAppCoreCurrentSpeed(float speed)
{
    emit doCurrentSpeed(speed);
}

void Controller::onDoNotForget()
{
    if (_isNotifyEnabled) {
        if (_isNotifyGraphicsEnabled) {
            emit doNotForgetGraphicsNotify();
        }
        if (_isNotifySoundEnabled) {
            emit doNotForgetSoundNotify();
        }
    }
}

#ifdef ANDROID
void Controller::keepScreenOn(bool on)
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

void Controller::checkItemLength(QString &item)
{
    int size = item.size();
    if (size > MAX_MARKS_ITEM_TEXT_LENGTH) {
        item.truncate(MAX_MARKS_ITEM_TEXT_LENGTH - 4);
        item.append(QString("..."));
    }
}
