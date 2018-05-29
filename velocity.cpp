#include "velocity.h"
#include "ui_velocity.h"

Velocity::Velocity(QWidget *parent) : QDialog(parent), ui(new Ui::Velocity)
{
    ui->setupUi(this);

    left = false;

    vel_y = 0;

    vel_x = 0;

    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(move_pin()));
}

void Velocity::keyPressEvent(QKeyEvent *e_)
{
    if (e_->key() == Qt::Key_P)
    {
        timer->stop();

        // validamos en que posición X quedó al aguja.
        if (ui->lbl_pin->x() <= 120)
        {
            if (vel_y == 0)
            {
                vel_y = 350;
            }
        }
        else if (ui->lbl_pin->x() > 120 && ui->lbl_pin->x() <= 140)
        {
            if (vel_y == 0)
            {
                vel_y = 280;
            }
        }
        else if (ui->lbl_pin->x() > 140 && ui->lbl_pin->x() <= 170)
        {
            if (vel_y == 0)
            {
                vel_y = 200;
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
        if (ui->lbl_pin->x() <= 160)
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
