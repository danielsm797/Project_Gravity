#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent) : QDialog(parent), ui(new Ui::LogIn)
{
    ui->setupUi(this);

    sc.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    sc.setAttribute(Qt::WA_TranslucentBackground);

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
}

void LogIn::show_message(int type_, QString mess_)
{
    if (type_ == 1)
    {
        QMessageBox msg(QMessageBox::Critical, "Log-In", mess_, QMessageBox::Ok, this);
        msg.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        msg.exec();
    }
    else if (type_ == 2)
    {
        QMessageBox msg(QMessageBox::Information, "Log-In", mess_, QMessageBox::Ok, this);
        msg.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        msg.exec();
    }
}

void LogIn::clear()
{
    user_name = "";

    pattern = "";

    ui->le_username->setText("");

    QIcon icon(QPixmap(":/Img/asteroid-1.png"));

    ui->btn_pat_1->setIcon(icon);
    ui->btn_pat_2->setIcon(icon);
    ui->btn_pat_3->setIcon(icon);
    ui->btn_pat_4->setIcon(icon);
    ui->btn_pat_5->setIcon(icon);
    ui->btn_pat_6->setIcon(icon);
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

    if (user_name.isEmpty()) // Validamos el nombre de usuario y contraseña.
    {
        show_message(1, "El nombre de usuario es requerido.");
        return;
    }

    if (pattern.isEmpty())
    {
        show_message(1, "La contraseña es requerida.");
        return;
    }

    if (users.length() < 2)
    {
        users.push_back(new User(user_name, pattern));

        if (is_login && !users.last()->getExist()) // El usuario no se encuentra creado.
        {
            show_message(1, "Nombre de usuario o contraseña incorrectos.");

            // Eliminamos el usuario.
            users.removeLast();

            return;
        }
        else if (!is_login && users.last()->getExist()) // Estamos creando el usuario y éste ya se encuentra registrado.
        {
            show_message(1, "Nombre de usuario existente.");

            // Eliminamos el usuario.
            users.removeLast();

            return;
        }
        else if (!is_login && !users.last()->getExist())
        {
            clear();

            show_message(2, "Usuario creado exitosamente!.");
        }
        else
        {
            clear();

            show_message(2, "Sesión iniciada.");

            ui->lbl_info->setText("Second Player");
        }

        if (!is_multiplayer || (is_multiplayer && users.length() == 2))
        {
            sc.show();

            this->close();
        }
    }
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
