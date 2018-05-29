#ifndef PLANET_H
#define PLANET_H


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

        Physics *fis;

    public:

        Planet(int pos_x_, int pos_y_, int width_, int height_, QString root_, float ace_gra_, float coe_fr_);

        float getAce_gra() const;

        void setAce_gra(float value);

        float getVel_x() const;

        void setVel_x(float value);

        float getVel_y() const;

        void setVel_y(float value);

        void mov_vertical();

        bool border(QGraphicsLineItem *l1, QGraphicsLineItem *l2, QGraphicsLineItem *l3, QGraphicsLineItem *l4);

        float getRadio() const;

        void setRadio(float value);
};

#endif // PLANET_H
