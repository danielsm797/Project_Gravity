#ifndef ELEMENT_H
#define ELEMENT_H

#include <QPainter>
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsScene>

class Element : public QGraphicsItem
{
private:

    int pos_x;

    int pos_y;

    int width;

    int height;

    int tipo;

    QString root;

public:

    Element(int pos_x_, int pos_y_, int width_, int height_, QString root_);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int getPos_x() const;

    void setPos_x(int value);

    int getPos_y() const;

    void setPos_y(int value);

    int getWidth() const;

    void setWidth(int value);

    int getHeight() const;

    void setHeight(int value);

    QString getRoot() const;

    void setRoot(const QString &value);

    void set_position();
};

#endif // ELEMENT_H
