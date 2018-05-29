#ifndef PHYSICS_H
#define PHYSICS_H

#include <QVector>
#include <math.h>

#define Dt 0.1

class Physics
{
    public:

        Physics();

        float mag_vel(float vx, float vy);

        float angulo_mov(float vx, float vy);

        QVector<float> cal_pos(float x_i, float y_i, float vx_i, float vy_i, float ac_x, float ac_y);

        QVector<float> cal_vel(float vx_i, float vy_i, float ac_x, float ac_y);

        QVector<float> cal_aceleracion(float mag_v, float rad, float masa, float angulo, float coe_fr, float ace_gra);
};

#endif // PHYSICS_H
