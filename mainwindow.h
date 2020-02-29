#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
class mainwindow;
}

class mainwindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainwindow(QString _id,QWidget *parent = 0);
    ~mainwindow();

private slots:
    void on_pushButton_download_clicked();

    void on_pushButton_choose_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_display_clicked();

private:
    Ui::mainwindow *ui;
    QString id;
};

#endif // MAINWINDOW_H
