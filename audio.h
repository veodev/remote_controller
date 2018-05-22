#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include <QMediaPlayer>

class Audio : public QObject
{
    Q_OBJECT
public:
    explicit Audio(QObject *parent = nullptr);

signals:

public slots:
    void initMedia();
    void playSound();
    void playDisconnected();

private:
    QMediaPlayer* _mediaPlayer;
};

#endif // AUDIO_H
