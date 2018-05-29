#ifndef LEVEL_H
#define LEVEL_H


#include <QVector>

class Level
{
    private:

        int id_level;

        int num_planets;

        float coe_res;

        float coe_vis;

        QVector<QString> targets; // Objetivos.

        QVector<QString> barrier; // Obstaculos.

    public:

        Level();

        int getId_level() const;

        void setId_level(int value);

        float getCoe_res() const;

        void setCoe_res(float value);

        float getCoe_vis() const;

        void setCoe_vis(float value);

        QVector<QString> getTargets() const;

        void setTargets(const QVector<QString> &value);

        QVector<QString> getBarrier() const;

        void setBarrier(const QVector<QString> &value);

        int getNum_planets() const;

        void setNum_planets(int value);
};

#endif // LEVEL_H
