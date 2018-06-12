#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include "user.h"
#include "scene.h"

using namespace std;

namespace Ui
{
class LogIn;
}

class LogIn : public QDialog
{
    Q_OBJECT

public:

    explicit LogIn(QWidget *parent = 0);

    void add_pattern(QPushButton *pb, QString str);

    void show_message(int type_, QString mess_);

    void clear();

    ~LogIn();

private slots:

    void on_btn_login_clicked();

    void on_btn_new_clicked();

    void on_btn_start_clicked();

    void on_btn_pat_1_clicked();

    void on_btn_pat_2_clicked();

    void on_btn_pat_4_clicked();

    void on_btn_pat_3_clicked();

    void on_btn_pat_5_clicked();

    void on_btn_pat_6_clicked();

public slots:

    void receive_data(bool value);

signals:

    void send_data(bool value);

private:

    bool is_login;

    bool is_multiplayer;

    QVector<User *> users;

    QString user_name;

    QString pattern;

    Scene sc;

    Ui::LogIn *ui;
};

#endif // LOGIN_H
