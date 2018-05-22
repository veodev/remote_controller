#include "audio.h"

#include <QDebug>

Audio::Audio(QObject *parent) : QObject(parent)
{
}

void Audio::initMedia()
{
    _mediaPlayer = new QMediaPlayer(this);
    _mediaPlayer->setMedia(QUrl("qrc:/sounds/WOWSENSOR-HIT.WAV"));
    _mediaPlayer->setVolume(100);
    qDebug() << "Media inited!";
}

void Audio::playSound()
{
    if (_mediaPlayer->state() != QMediaPlayer::PlayingState) {
        _mediaPlayer->setPosition(0);
        _mediaPlayer->play();
    }
}
