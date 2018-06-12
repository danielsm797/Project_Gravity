#include "planet.h"

float Planet::getAce_gra() const
{
    return ace_gra;
}

void Planet::setAce_gra(float value)
{
    ace_gra = value;
}

float Planet::getVel_x() const
{
    return vel_x;
}

void Planet::setVel_x(float value)
{
    vel_x = value;
}

float Planet::getVel_y() const
{
    return vel_y;
}

void Planet::setVel_y(float value)
{
    vel_y = value;
}

void Planet::mov_vertical(bool fl)
{
    float mag = fis->mag_vel(vel_x, vel_y);

    float ang = fis->angulo_mov(vel_x, vel_y);

    QVector<float> ace(fis->cal_aceleracion(mag, radio, masa, ang, coe_fr, ace_gra));

    QVector<float> pos(fis->cal_pos(getPos_x(), getPos_y(), vel_x, vel_y, ace.at(0), ace.at(1)));

    QVector<float> pos_(fis->cal_pos(getPos_x(), y_aux, vel_x, vel_y, ace.at(0), ace.at(1))); // Posición auxiliar.

    if (!fl)
    {
        setPos_x(pos.at(0));

        if (pos_.at(1) < y_aux)
        {
            setPos_y(pos.at(1) + ((getPos_y()- pos.at(1)) + 1));
        }
        else
        {
            setPos_y(pos.at(1) - ((pos.at(1) - getPos_y()) + 1));
        }

        y_aux = pos_.at(1);
    }
    else
    {
        setPos_x(pos.at(0));

        setPos_y(pos.at(1));
    }

    QVector<float> vel(fis->cal_vel(vel_x, vel_y, ace.at(0), ace.at(1)));

    vel_x = vel.at(0);

    vel_y = vel.at(1);

    set_position();
}

void Planet::mov_circular()
{
    angle++;

    float w = fis->cal_vel_ang(angle, 120);

    double orig = 0.01;

    float f = (float) orig;

    QVector<float> resp = fis->cal_pos_cir(120, w, origin_x, origin_y, f);

    setPos_x(resp.first());

    setPos_y(resp.last());

    set_position();

    if (angle >= 450)
    {
        mode = 1;

        angle = 90;
    }
}

void Planet::mov_ondulatorio()
{
    QVector<float> resp = fis->cal_pos_ond(90, angle);

    setPos_x(resp.first());

    setPos_y(random_y + resp.last());

    angle++;

    set_position();

    if (angle >= 450)
    {
        mode = 1;

        angle = 90;
    }
}

bool Planet::border(QMediaPlayer *mp)
{
    bool resp = false;

    if (getPos_x() < 11)
    {
        setPos_x(11);

        vel_x *= (-1 * coe_re);

        mp->play();
    }
    else if (getPos_x() > 700)
    {
        setPos_x(700);

        vel_x *= (-1 * coe_re);

        mp->play();
    }
    else if (getPos_y() < 11)
    {
        setPos_y(11);

        vel_y *= (-1 * coe_re);

        mp->play();
    }
    else if (getPos_y() > 540)
    {
        setPos_y(540);

        vel_y *= (-1 * coe_re);

        mp->play();
    }

    if (abs(vel_x) < 1 || abs(vel_y) < 1)
    {
        resp = true; // Terminó.
    }

    return resp;
}

float Planet::getRadio() const
{
    return radio;
}

void Planet::setRadio(float value)
{
    radio = value;
}

float Planet::getCoe_fr() const
{
    return coe_fr;
}

void Planet::setCoe_fr(float value)
{
    coe_fr = value;
}

float Planet::getY_aux() const
{
    return y_aux;
}

void Planet::setY_aux(float value)
{
    y_aux = value;
}

int Planet::getMode() const
{
    return mode;
}

void Planet::setMode(int value)
{
    mode = value;
}

float Planet::getOrigin_x() const
{
    return origin_x;
}

void Planet::setOrigin_x(float value)
{
    origin_x = value;
}

float Planet::getOrigin_y() const
{
    return origin_y;
}

void Planet::setOrigin_y(float value)
{
    origin_y = value;
}

float Planet::getAngle() const
{
    return angle;
}

void Planet::setAngle(float value)
{
    angle = value;
}

Planet::Planet(int pos_x_, int pos_y_, int width_, int height_, QString root_, float ace_gra_, float coe_fr_, float coe_rt_) : Element(pos_x_, pos_y_, width_, height_, root_)
{
    ace_gra = ace_gra_;

    coe_fr = coe_fr_;

    coe_re = coe_rt_;

    vel_x = 0;

    angle = 90;

    masa = 577;

    radio = 1.5;

    origin_x = 0;

    origin_y = 0;

    mode = 1; // Modo normal, 2: Modo circular y 3: Modo Pendular.

    random_y = 100 + qrand() % 350;

    y_aux = getPos_y();
}
