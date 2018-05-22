#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent) : QDialog(parent), ui(new Ui::LogIn)
{
    ui->setupUi(this);

    is_login = true;

    connect(this, SIGNAL(send_data(bool)), this, SLOT(receive_data(bool)));
}

void LogIn::add_pattern(QPushButton *pb, QString str)
{
    if (!pattern.contains(str))
    {
        pattern.append(str);
        pb->setIcon(QIcon(QPixmap(":/Img/asteroid-2.png")));
    }
    else
    {
        pattern.remove(str);
        pb->setIcon(QIcon(QPixmap(":/Img/asteroid-1.png")));
    }

    qDebug() << pattern;
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::on_btn_login_clicked()
{
    QString q_str_new("QPushButton {color: yellow; border: 1px solid yellow;}"
                  "QPushButton:hover {background-color: rgba(255, 255, 0, .2);}"
                  "QPushButton:pressed {color: #F3A71E;border: 1px solid #F3A71E;background-color: rgba(0, 0, 0, .4);}");

    QString q_str_old("QPushButton {color: white; border: 1px solid white;}"
                  "QPushButton:hover {background-color: rgba(0, 187, 193, .4);}"
                  "QPushButton:pressed {color: #F3A71E;border: 1px solid #F3A71E;background-color: rgba(0, 0, 0, .4);}");

    ui->btn_login->setStyleSheet(q_str_new);

    ui->btn_new->setStyleSheet(q_str_old);

    is_login = true;
}

void LogIn::on_btn_new_clicked()
{
    QString q_str_new("QPushButton {color: yellow; border: 1px solid yellow;}"
                  "QPushButton:hover {background-color: rgba(255, 255, 0, .2);}"
                  "QPushButton:pressed {color: #F3A71E;border: 1px solid #F3A71E;background-color: rgba(0, 0, 0, .4);}");

    QString q_str_old("QPushButton {color: white; border: 1px solid white;}"
                  "QPushButton:hover {background-color: rgba(0, 187, 193, .4);}"
                  "QPushButton:pressed {color: #F3A71E;border: 1px solid #F3A71E;background-color: rgba(0, 0, 0, .4);}");

    ui->btn_login->setStyleSheet(q_str_old);

    ui->btn_new->setStyleSheet(q_str_new);

    is_login = false;
}

void LogIn::on_btn_start_clicked()
{
    user_name = ui->le_username->text();
}

void LogIn::receive_data(bool value)
{
    is_multiplayer = value;
}

void LogIn::on_btn_pat_1_clicked()
{
    add_pattern(ui->btn_pat_1, "1");
}

void LogIn::on_btn_pat_2_clicked()
{
    add_pattern(ui->btn_pat_2, "2");
}

void LogIn::on_btn_pat_4_clicked()
{
    add_pattern(ui->btn_pat_4, "4");
}

void LogIn::on_btn_pat_3_clicked()
{
    add_pattern(ui->btn_pat_3, "3");
}

void LogIn::on_btn_pat_5_clicked()
{
    add_pattern(ui->btn_pat_5, "5");
}

void LogIn::on_btn_pat_6_clicked()
{
    add_pattern(ui->btn_pat_6, "6");
}
