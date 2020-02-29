#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include <QWidget>

namespace Ui {
class updatewindow;
}

class updatewindow : public QWidget
{
    Q_OBJECT

public:
    explicit updatewindow(QWidget *parent = 0);
    ~updatewindow();

private slots:
    void on_pushButton_update_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::updatewindow *ui;
};

#endif // UPDATEWINDOW_H
