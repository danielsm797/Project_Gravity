#include "velocity.h"
#include "ui_velocity.h"

Velocity::Velocity(QWidget *parent) : QDialog(parent), ui(new Ui::Velocity)
{
    ui->setupUi(this);

    left = false;

    vel_y = 0;

    vel_x = 0;

    y = true;

    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(move_pin()));
}

void Velocity::keyPressEvent(QKeyEvent *e_)
{
    if (e_->key() == Qt::Key_P)
    {
        timer->stop();

        // validamos en que posición X quedó al aguja.
        if (ui->lbl_pin->x() < 80)
        {
            if (vel_y == 0 && y)
            {
                vel_y = 90;
            }

            if (!y)
            {
                vel_x = -400;

                vel_y = 400;
            }
        }
        else if (ui->lbl_pin->x() >= 80 && ui->lbl_pin->x() < 110)
        {
            if (vel_y == 0 && y)
            {
                vel_y = 90;
            }

            if (!y)
            {
                vel_x = -500;

                vel_y = 500;
            }
        }
        else if (ui->lbl_pin->x() >= 110 && ui->lbl_pin->x() < 130)
        {
            if (vel_y == 0 && y)
            {
                vel_y = 60;
            }

            if (!y)
            {
                vel_x = -300;

                vel_y = -300;
            }
        }
        else if (ui->lbl_pin->x() >= 130 && ui->lbl_pin->x() < 160)
        {
            if (vel_y == 0 && y)
            {
                vel_y = 90;
            }

            if (!y)
            {
                vel_x = -600;

                vel_y = 600;
            }
        }
        else if (ui->lbl_pin->x() >= 160)
        {
            if (vel_y == 0 && y)
            {
                vel_y = 90;
            }

            if (!y)
            {
                vel_x = -450;

                vel_y = 450;
            }
        }

        this->close();
    }
}

Velocity::~Velocity()
{
    delete ui;

    delete timer;
}

void Velocity::move_pin()
{
    if (!left)
    {
        if (ui->lbl_pin->x() <= 180)
        {
            ui->lbl_pin->move(ui->lbl_pin->x() + 1, ui->lbl_pin->y());
        }
        else
        {
            left = true;
        }
    }
    else
    {
        if (ui->lbl_pin->x() > 70)
        {
            ui->lbl_pin->move(ui->lbl_pin->x() - 1, ui->lbl_pin->y());
        }
        else
        {
            left = false;
        }
    }
}

bool Velocity::getY() const
{
    return y;
}

void Velocity::setY(bool value)
{
    y = value;
}

float Velocity::getVel_y() const
{
    return vel_y;
}

void Velocity::setVel_y(float value)
{
    vel_y = value;
}

float Velocity::getVel_x() const
{
    return vel_x;
}

void Velocity::setVel_x(float value)
{
    vel_x = value;
}

QTimer *Velocity::getTimer() const
{
    return timer;
}

void Velocity::setTimer(QTimer *value)
{
    timer = value;
}
