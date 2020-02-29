#ifndef REGISTWINDOW_H
#define REGISTWINDOW_H

#include <QWidget>

namespace Ui {
class registwindow;
}

class registwindow : public QWidget
{
    Q_OBJECT

public:
    explicit registwindow(QWidget *parent = 0);
    ~registwindow();

private slots:
    void on_pushButton_register_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::registwindow *ui;
};

#endif // REGISTWINDOW_H
