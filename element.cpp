#include "element.h"

int Element::getPos_x() const
{
    return pos_x;
}

void Element::setPos_x(int value)
{
    pos_x = value;
}

int Element::getPos_y() const
{
    return pos_y;
}

void Element::setPos_y(int value)
{
    pos_y = value;
}

int Element::getWidth() const
{
    return width;
}

void Element::setWidth(int value)
{
    width = value;
}

int Element::getHeight() const
{
    return height;
}

void Element::setHeight(int value)
{
    height = value;
}

QString Element::getRoot() const
{
    return root;
}

void Element::setRoot(const QString &value)
{
    root = value;
}

void Element::set_position()
{
    setPos(pos_x, pos_y);
}

Element::Element(int pos_x_, int pos_y_, int width_, int height_, QString root_)
{
    pos_x = pos_x_;

    pos_y = pos_y_;

    width = width_;

    height = height_;

    root = root_;
}

QRectF Element::boundingRect() const
{
    return QRectF(-10, -10, width, height);
}

void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap px;
    px.load(root);
    painter->drawPixmap(boundingRect(), px, px.rect());
}
