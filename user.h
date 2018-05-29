#ifndef USER_H
#define USER_H

#define DOC_NAME "Users_doc.txt"

#include <QString>
#include <QStringList>
#include <fstream>

using namespace std;

class User
{
    private:

        int id_user;

        int id_lastGame;

        bool exist;

        QString user_name;

        QString password;

        QString dataf;

    public:

        User();

        User(QString user_name_, QString password_);

        QString getUser_name() const;

        void read_file();

        void save_user();

        int getId_user() const;

        void setId_user(int value);

        int getId_lastGame() const;

        void setId_lastGame(int value);

        bool getExist() const;
};

#endif // USER_H
