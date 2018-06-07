#include "physics.h"

Physics::Physics()
{

}

float Physics::mag_vel(float vx, float vy)
{
    float resp = 0;

    resp = pow(vx, 2) + pow(vy, 2);

    return resp;
}

float Physics::angulo_mov(float vx, float vy)
{
    float resp = 0;

    resp = atan2(vy, vx);

    return resp;
}

QVector<float> Physics::cal_pos(float x_i, float y_i, float vx_i, float vy_i, float ac_x, float ac_y)
{
    QVector<float> resp;

    resp.append(x_i + (vx_i * Dt) + ((ac_x * pow(Dt, 2)) / 2));

    resp.append(y_i + (vy_i * Dt) + ((ac_y * pow(Dt, 2)) / 2));

    return resp;
}

QVector<float> Physics::cal_vel(float vx_i, float vy_i, float ac_x, float ac_y)
{
    QVector<float> resp;

    resp.append(vx_i + (ac_x * Dt));

    resp.append(vy_i + (ac_y * Dt));

    return resp;
}

QVector<float> Physics::cal_aceleracion(float mag_v, float rad, float masa, float angulo, float coe_fr, float ace_gra)
{
    QVector<float> resp;

    resp.append(
        (-1 * (coe_fr * mag_v * pow(rad, 2)) / masa) * cosf(angulo)
    );

    resp.append(
        (
            (-1 * (coe_fr * mag_v * pow(rad, 2)) / masa) * sinf(angulo)
        ) - ace_gra
    );

    return resp;
}

// Movimiento circular.

QVector<float> Physics::cal_pos_cir(float rad, float w, float x, float y, float t)
{
    QVector<float> resp;

    resp.append(x + rad * cos(w * t));

    resp.append(y + rad * sin(w * t));

    return resp;
}

float Physics::cal_vel_ang(float ang, float rad)
{
    float resp = 0;

    resp = (2 * M_PI * rad * ang) / 360;

    return resp;
}

