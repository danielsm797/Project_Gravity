#include "taco.h"

Taco::Taco(int pos_x_, int pos_y_, int width_, int height_, QString root_) : Element(pos_x_, pos_y_, width_, height_, root_)
{

}

void Taco::hor_move()
{
    if (getPos_y() <= 608)
    {
        setPos_y(getPos_y() + 1);
    }
    else
    {
        setPos_y(580);
    }

    set_position();
}

void Taco::ver_move()
{
    if (getPos_x() <= 783)
    {
        setPos_x(getPos_x() + 1);
    }
    else
    {
        setPos_x(775);
    }

    set_position();
}
