#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_register_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
