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

void Planet::mov_vertical()
{
    float mag = fis->mag_vel(vel_x, vel_y);

    float ang = fis->angulo_mov(vel_x, vel_y);

    QVector<float> ace(fis->cal_aceleracion(mag, radio, masa, ang, coe_fr, ace_gra));

    QVector<float> pos(fis->cal_pos(getPos_x(), getPos_y(), vel_x, vel_y, ace.at(0), ace.at(1)));

    QVector<float> pos_(fis->cal_pos(getPos_x(), y_aux, vel_x, vel_y, ace.at(0), ace.at(1))); // Posición auxiliar.

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

    QVector<float> vel(fis->cal_vel(vel_x, vel_y, ace.at(0), ace.at(1)));

    vel_x = vel.at(0);

    vel_y = vel.at(1);

    set_position();
}

bool Planet::border(QGraphicsLineItem *l1, QGraphicsLineItem *l2, QGraphicsLineItem *l3, QGraphicsLineItem *l4)
{
    bool resp = false;

    if (this->collidesWithItem(l1) || this->collidesWithItem(l4))
    {
        vel_y *= (-1 * coe_re);

        resp = true;
    }
    else if (this->collidesWithItem(l2) || this->collidesWithItem(l3))
    {
        vel_x *= (-1 * coe_re);

        resp = true;
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

Planet::Planet(int pos_x_, int pos_y_, int width_, int height_, QString root_, float ace_gra_, float coe_fr_) : Element(pos_x_, pos_y_, width_, height_, root_)
{
    ace_gra = ace_gra_;

    coe_fr = coe_fr_;

    coe_re = 0.5;

    vel_x = 0;

    masa = 577.642151;

    radio = 1.5;

    y_aux = getPos_y();
}
