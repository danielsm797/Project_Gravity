#ifndef PHYSICS_H
#define PHYSICS_H

#define _USE_MATH_DEFINES
#include <QVector>
#include <math.h>

#define Dt 0.01

class Physics
{
public:

    Physics();

    float mag_vel(float vx, float vy);

    float angulo_mov(float vx, float vy);

    QVector<float> cal_pos(float x_i, float y_i, float vx_i, float vy_i, float ac_x, float ac_y);

    QVector<float> cal_vel(float vx_i, float vy_i, float ac_x, float ac_y);

    QVector<float> cal_aceleracion(float mag_v, float rad, float masa, float angulo, float coe_fr, float ace_gra);

    QVector<float> cal_pos_cir(float rad, float w, float x, float y, float t);

    float cal_vel_ang(float ang, float rad);

    QVector<float> cal_pos_ond(float a, float y);
};

#endif // PHYSICS_H
