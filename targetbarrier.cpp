#include "targetbarrier.h"

int TargetBarrier::getScore() const
{
    return score;
}

void TargetBarrier::setScore(int value)
{
    score = value;
}

int TargetBarrier::getType() const
{
    return type;
}

void TargetBarrier::setType(int value)
{
    type = value;
}

void TargetBarrier::play_sound()
{
    sound->play();
}

void TargetBarrier::move_cir()
{
    angulo++;

    float w = phy.cal_vel_ang(angulo, 35);

    QVector<float> resp = phy.cal_pos_cir(35, w, origen_x, origen_y, 0.2);

    setPos_x(resp.first());

    setPos_y(resp.last());

    set_position();
}

TargetBarrier::~TargetBarrier()
{
    delete sound;
}

float TargetBarrier::getAngulo() const
{
    return angulo;
}

void TargetBarrier::setAngulo(float value)
{
    angulo = value;
}

float TargetBarrier::getOrigen_x() const
{
    return origen_x;
}

void TargetBarrier::setOrigen_x(float value)
{
    origen_x = value;
}

float TargetBarrier::getOrigen_y() const
{
    return origen_y;
}

void TargetBarrier::setOrigen_y(float value)
{
    origen_y = value;
}

TargetBarrier::TargetBarrier(int pos_x_, int pos_y_, int width_, int height_, QString root_, int score_, int type_, int ang_) : Element(pos_x_, pos_y_, width_, height_, root_)
{
    score = score_;

    type = type_;

    origen_x = pos_x_;

    origen_y = pos_y_ + 15;

    angulo = ang_;

    sound = new QMediaPlayer();

    if (type_ == 2)
    {
        sound->setMedia(QUrl("qrc:/Sounds/good.mp3"));
    }
    else if (type_ == 1)
    {
        sound->setMedia(QUrl("qrc:/Sounds/bad.mp3"));
    }
    else
    {
        sound->setMedia(QUrl("qrc:/Sounds/pow.mp3"));
    }
}
