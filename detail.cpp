#include "detail.h"
#include "ui_detail.h"

Detail::Detail(QWidget *parent) : QDialog(parent), ui(new Ui::Detail)
{
    ui->setupUi(this);

    next = false;
}

void Detail::set_score(int one, int two, int can, bool is_)
{
    ui->lcd_score1->display(one);

    ui->lcd_score2->display(two);

    if (!is_)
    {
        if (can > 0)
        {
            ui->lbl_info->setText("Game over");

            ui->btn_next->setText("Repeat");
        }
        else
        {
            ui->lbl_info->setText("You win");
        }

        return;
    }

    if (one > two)
    {
        ui->lbl_info->setText("Player one win");
    }
    else if (two > one)
    {
        ui->lbl_info->setText("Player two win");
    }
    else
    {
        ui->lbl_info->setText("Hay un empate");
    }
}

void Detail::on_btn_next_clicked()
{
    next = true;

    ui->btn_next->setText("Next level");

    this->close();
}

bool Detail::getNext() const
{
    return next;
}

void Detail::setNext(bool value)
{
    next = value;
}

void Detail::on_btn_close_clicked()
{
    next = false;
}

Detail::~Detail()
{
    delete ui;
}
