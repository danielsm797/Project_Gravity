#ifndef TARGETBARRIER_H
#define TARGETBARRIER_H


#include "element.h"

class TargetBarrier : public Element
{
    private:

        int score;

        int type; // 1 Barrera, 2 Objetivo, 3 move para, 4 move ondu.

    public:

        TargetBarrier(int pos_x_, int pos_y_, int width_, int height_, QString root_, int score_, int type_);

        int getScore() const;

        void setScore(int value);

        int getType() const;

        void setType(int value);
};

#endif // TARGETBARRIER_H