#ifndef DETAIL_H
#define DETAIL_H

#include <QDialog>

namespace Ui {
class Detail;
}

class Detail : public QDialog
{
    Q_OBJECT

public:

    explicit Detail(QWidget *parent = 0);

    void set_score(int one, int two, int can, bool is_);

    bool getNext() const;

    void setNext(bool value);

    ~Detail();

private slots:

    void on_btn_next_clicked();

    void on_btn_close_clicked();

private:

    bool next;

    Ui::Detail *ui;
};

#endif // DETAIL_H
