#include "user.h"

QString User::getUser_name() const
{
    return user_name;
}

int User::getId_user() const
{
    return id_user;
}

void User::setId_user(int value)
{
    id_user = value;
}

int User::getId_lastGame() const
{
    return id_lastGame;
}

void User::setId_lastGame(int value)
{
    id_lastGame = value;
}

bool User::getExist() const
{
    return exist;
}

User::User(QString user_name_, QString password_)
{
    user_name = user_name_;

    password = password_;

    id_user = 0;

    id_lastGame = 0;

    exist = false;

    dataf = "";

    read_file();
}

void User::read_file()
{
    ifstream ifs;
    ifs.open(DOC_NAME);

    if (ifs.is_open())
    {
        char c;
        while (ifs.get(c))
        {
            dataf.append(c);
        }
    }

    QStringList users_list = dataf.split('|');

    if (!dataf.isEmpty())
    {
        QStringList::iterator it = users_list.begin();

        for (; it != users_list.end(); it++) // Recorremos los usuarios.
        {
            QString str = *it;

            QStringList info_list = str.split('@');

            QString str_name = info_list.at(1);
            QString str_pass = info_list.at(2);

            if (str_name == user_name && str_pass == password) // Validamos usuario y contraseña.
            {
                QString id_usr = info_list.first();
                id_user = id_usr.toInt();

                QString id_lag = info_list.last();
                id_lastGame = id_lag.toInt();

                break;
            }
        }
    }

    if (id_user > 0) // El usuario se encuentra creado.
    {
        exist = true;
    }
    else // Ponemos el id del usuario.
    {
        QString last_user = users_list.last();
        QString id_usr = last_user.split('@').first();

        id_user = id_usr.toInt() + 1;

        save_user();
    }
}

void User::save_user()
{
    ofstream ofs;
    ofs.open(DOC_NAME, ios::app);

    if (ofs.is_open())
    {
        if (dataf.isEmpty())
        {
            ofs << id_user << "@" << user_name.toStdString() << "@" << password.toStdString() << "@" << id_lastGame;
        }
        else
        {
            ofs << "|" << id_user << "@" << user_name.toStdString() << "@" << password.toStdString() << "@" << id_lastGame;
        }
    }

    ofs.close();
}
