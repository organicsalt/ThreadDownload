#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "registwindow.h"
#include "updatewindow.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->textpassword->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_pushButton_login_clicked()
{
    QSqlDatabase sqlConn;
    sqlConn = QSqlDatabase::addDatabase(tr("QMYSQL"));
    sqlConn.setHostName(tr("localhost")); //主机名，或者填写127.0.0.1
    sqlConn.setPort(3306); //mysql的端口号
    sqlConn.setUserName(tr("root")); //mysql用户名
    sqlConn.setPassword(tr("123456")); //密码
    sqlConn.setDatabaseName(tr("dbem")); //要操作的数据库
    bool b = sqlConn.open();
    qDebug()<<b;
    if(!b) //判断是否连接成功
    {
       qDebug()<<tr("数据库连接失败").toUtf8().data();
    }
    else
    {
       qDebug()<<tr("数据库连接成功").toUtf8().data();
    }
    QSqlQuery sqlQuery;
    sqlQuery.prepare(tr("select password from user where id = ?"));
    sqlQuery.addBindValue(ui->textid->text()); //添加用户名
    if(!sqlQuery.exec()) //执行语句
    {
        qDebug()<<tr("查询失败！").toUtf8().data();
    }
    else
    {
        if(sqlQuery.first()){
            //如果有数据，循环读取，把数据显示出来
            qDebug()<<sqlQuery.value(0).toString();
            if(ui->textpassword->text()==sqlQuery.value(0).toString()){
                QMessageBox::information(NULL, "提示", "登录成功");
                mainwindow *w=new mainwindow(ui->textid->text());
                w->setWindowTitle("主界面");
                w->show();
                this->close();
            }
            else{
               QMessageBox::information(NULL, "提示", "用户名密码错误！");
            }
        }
    }
    sqlConn.close();
}


void LoginWindow::on_pushButton_update_clicked()
{
    registwindow *w=new registwindow();
    w->setWindowTitle("注册界面");
    w->show();
    this->close();
}

void LoginWindow::on_pushButton_exit_clicked()
{
    this->close();
}

void LoginWindow::on_pushButton_register_clicked()
{
    updatewindow *w=new updatewindow();
    w->setWindowTitle("修改界面");
    w->show();
    this->close();
}
