#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow(QWidget *parent = 0);

        ~MainWindow();

    private slots:

        void on_btn_multiplayer_clicked();

        void on_btn_singleplayer_clicked();

        void on_btn_next_clicked();

    signals:

        void open_login();

    private:

        bool is_multiplayer = false;

        Ui::MainWindow *ui;

        LogIn li;
};

#endif // MAINWINDOW_H
