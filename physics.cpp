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

    resp.push_back(x_i + (vx_i * Dt) + ((ac_x * pow(Dt, 2)) / 2));

    resp.push_back(y_i + (vy_i * Dt) + ((ac_y * pow(Dt, 2)) / 2));

    return resp;
}

QVector<float> Physics::cal_vel(float vx_i, float vy_i, float ac_x, float ac_y)
{
    QVector<float> resp;

    resp.push_back(vx_i + (ac_x * Dt));

    resp.push_back(vy_i + (ac_y * Dt));

    return resp;
}

QVector<float> Physics::cal_aceleracion(float mag_v, float rad, float masa, float angulo, float coe_fr, float ace_gra)
{
    QVector<float> resp;

    resp.push_back(
        (-1 * (coe_fr * mag_v * pow(rad, 2)) / masa) * cosf(angulo)
    );

    resp.push_back(
        (
            (-1 * (coe_fr * mag_v * pow(rad, 2)) / masa) * sinf(angulo)
        ) - ace_gra
    );

    return resp;
}
