#include "detail.h"
#include "ui_detail.h"

Detail::Detail(QWidget *parent) : QDialog(parent), ui(new Ui::Detail)
{
    ui->setupUi(this);

    next = false;
}

void Detail::set_score(int one, int two)
{
    ui->lcd_score1->display(one);

    ui->lcd_score2->display(two);

    if (one > two)
    {
        ui->lbl_info->setText("PLAYER ONE WIN");
    }
    else if (two > one)
    {
        ui->lbl_info->setText("PLAYER TWO WIN");
    }
    else
    {
        ui->lbl_info->setText("HAY UN EMPATE");
    }
}

void Detail::on_btn_next_clicked()
{
    next = true;

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
