#ifndef PLANET_H
#define PLANET_H

#include <QDebug>
#include <QString>
#include <QMediaPlayer>
#include "physics.h"
#include "element.h"

class Planet : public Element
{
private:

    float vel_x;

    float vel_y;

    float ace_gra;

    float masa;

    float radio;

    float coe_fr;

    float coe_re;

    float y_aux;

    float origin_x;

    float origin_y;

    float angle;

    int mode;

    int random_y;

    Physics *fis;

public:

    Planet(int pos_x_, int pos_y_, int width_, int height_, QString root_, float ace_gra_, float coe_fr_, float coe_rt_);

    float getAce_gra() const;

    void setAce_gra(float value);

    float getVel_x() const;

    void setVel_x(float value);

    float getVel_y() const;

    void setVel_y(float value);

    void mov_vertical(bool fl);

    void mov_circular();

    void mov_ondulatorio();

    bool border(QMediaPlayer *mp);

    float getRadio() const;

    void setRadio(float value);

    float getCoe_fr() const;

    void setCoe_fr(float value);

    float getY_aux() const;

    void setY_aux(float value);

    int getMode() const;

    void setMode(int value);

    float getOrigin_x() const;

    void setOrigin_x(float value);

    float getOrigin_y() const;

    void setOrigin_y(float value);

    float getAngle() const;

    void setAngle(float value);
};

#endif // PLANET_H
