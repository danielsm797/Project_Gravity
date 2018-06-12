#ifndef TARGETBARRIER_H
#define TARGETBARRIER_H


#include "element.h"
#include "physics.h"
#include <QString>
#include <QMediaPlayer>

class TargetBarrier : public Element
{
private:

    int score;

    int type; // 1 Barrera, 2 Objetivo, 3 move para, 4 move ondu.

    float origen_x;

    float origen_y;

    float angulo;

    Physics phy;

    QMediaPlayer *sound;

public:

    TargetBarrier(int pos_x_, int pos_y_, int width_, int height_, QString root_, int score_, int type_, int ang_);

    int getScore() const;

    void setScore(int value);

    int getType() const;

    void setType(int value);

    void play_sound();

    void move_cir();

    ~TargetBarrier();

    float getAngulo() const;

    void setAngulo(float value);

    float getOrigen_x() const;

    void setOrigen_x(float value);

    float getOrigen_y() const;

    void setOrigen_y(float value);
};

#endif // TARGETBARRIER_H
