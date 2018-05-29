#ifndef TARGETBARRIER_H
#define TARGETBARRIER_H


#include "element.h"

class TargetBarrier : public Element
{
    private:

        int score;

        bool type; // 1 Barrera, 2 Objetivo.

    public:

        TargetBarrier(int pos_x_, int pos_y_, int width_, int height_, QString root_, int score_, bool type_);

        int getScore() const;

        void setScore(int value);

        bool getType() const;

        void setType(bool value);
};

#endif // TARGETBARRIER_H
