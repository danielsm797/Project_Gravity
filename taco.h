#ifndef TACO_H
#define TACO_H

#include "element.h"
#include <QString>

class Taco : public Element
{
public:

    Taco(int pos_x_, int pos_y_, int width_, int height_, QString root_);

public slots:

    void hor_move();

    void ver_move();
};

#endif // TACO_H
