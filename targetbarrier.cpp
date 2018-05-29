#include "targetbarrier.h"

int TargetBarrier::getScore() const
{
    return score;
}

void TargetBarrier::setScore(int value)
{
    score = value;
}

int TargetBarrier::getType() const
{
    return type;
}

void TargetBarrier::setType(int value)
{
    type = value;
}

TargetBarrier::TargetBarrier(int pos_x_, int pos_y_, int width_, int height_, QString root_, int score_, int type_) : Element(pos_x_, pos_y_, width_, height_, root_)
{
    score = score_;

    type = type_;
}