#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    li.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    li.setAttribute(Qt::WA_TranslucentBackground);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_multiplayer_clicked()
{
    QString q_str_new("QPushButton {color: yellow; border: 1px solid yellow;}"
                  "QPushButton:hover {background-color: rgba(255, 255, 0, .2);}"
                  "QPushButton:pressed {color: #F3A71E;border: 1px solid #F3A71E;background-color: rgba(0, 0, 0, .4);}");

    QString q_str_old("QPushButton {color: white; border: 1px solid white;}"
                  "QPushButton:hover {background-color: rgba(0, 187, 193, .4);}"
                  "QPushButton:pressed {color: #F3A71E;border: 1px solid #F3A71E;background-color: rgba(0, 0, 0, .4);}");

    ui->btn_multiplayer->setStyleSheet(q_str_new);

    ui->btn_singleplayer->setStyleSheet(q_str_old);

    is_multiplayer = true;
}

void MainWindow::on_btn_singleplayer_clicked()
{
    QString q_str_new("QPushButton {color: yellow; border: 1px solid yellow;}"
                  "QPushButton:hover {background-color: rgba(255, 255, 0, .2);}"
                  "QPushButton:pressed {color: #F3A71E;border: 1px solid #F3A71E;background-color: rgba(0, 0, 0, .4);}");

    QString q_str_old("QPushButton {color: white; border: 1px solid white;}"
                  "QPushButton:hover {background-color: rgba(0, 187, 193, .4);}"
                  "QPushButton:pressed {color: #F3A71E;border: 1px solid #F3A71E;background-color: rgba(0, 0, 0, .4);}");

    ui->btn_multiplayer->setStyleSheet(q_str_old);

    ui->btn_singleplayer->setStyleSheet(q_str_new);

    is_multiplayer = false;
}

void MainWindow::on_btn_next_clicked()
{
    li.show();

    emit li.send_data(is_multiplayer);

    this->close();
}
