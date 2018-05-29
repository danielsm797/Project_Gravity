#include "level.h"

int Level::getId_level() const
{
    return id_level;
}

void Level::setId_level(int value)
{
    id_level = value;
}

float Level::getCoe_res() const
{
    return coe_res;
}

void Level::setCoe_res(float value)
{
    coe_res = value;
}

float Level::getCoe_vis() const
{
    return coe_vis;
}

void Level::setCoe_vis(float value)
{
    coe_vis = value;
}

QVector<QString> Level::getTargets() const
{
    return targets;
}

void Level::setTargets(const QVector<QString> &value)
{
    targets = value;
}

QVector<QString> Level::getBarrier() const
{
    return barrier;
}

void Level::setBarrier(const QVector<QString> &value)
{
    barrier = value;
}

int Level::getNum_planets() const
{
    return num_planets;
}

void Level::setNum_planets(int value)
{
    num_planets = value;
}

Level::Level()
{

}
