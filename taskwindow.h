#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QWidget>

namespace Ui {
class taskwindow;
}

class taskwindow : public QWidget
{
    Q_OBJECT

public:
    explicit taskwindow(QString id,QWidget *parent = 0);
    ~taskwindow();

private slots:


    void on_pushButton_deleterecord_clicked();

    void on_pushButton_looktask_clicked();

    void on_table_clicked(const QModelIndex &index);

    void on_pushButton_openpath_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_redownload_clicked();

private:
    Ui::taskwindow *ui;
    QString id="";
    QString name="";
    QString path="";
    QString URL="";
};

#endif // TASKWINDOW_H
