#ifndef VELOCITY_H
#define VELOCITY_H

#include <QDialog>
#include <QTimer>
#include <QKeyEvent>

namespace Ui
{
class Velocity;
}

class Velocity : public QDialog
{
    Q_OBJECT

public:

    explicit Velocity(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *e_);

    ~Velocity();

    QTimer *getTimer() const;

    void setTimer(QTimer *value);

    float getVel_x() const;

    void setVel_x(float value);

    float getVel_y() const;

    void setVel_y(float value);

    bool getY() const;

    void setY(bool value);

public slots:

    void move_pin();

private:

    bool left;

    float vel_x;

    float vel_y;

    bool y;

    QTimer *timer;

    Ui::Velocity *ui;
};

#endif // VELOCITY_H
