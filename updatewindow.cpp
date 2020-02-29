#include "updatewindow.h"
#include "ui_updatewindow.h"
#include "loginwindow.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

updatewindow::updatewindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updatewindow)
{
    ui->setupUi(this);
    ui->textoldpassword->setEchoMode(QLineEdit::Password);
    ui->textnewpassword->setEchoMode(QLineEdit::Password);
}

updatewindow::~updatewindow()
{
    delete ui;
}

void updatewindow::on_pushButton_update_clicked()
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
            if(ui->textoldpassword->text()==sqlQuery.value(0).toString()){
                sqlQuery.prepare(tr("update user set password = ? where id = ?"));
                sqlQuery.bindValue(0,ui->textnewpassword->text()); //添加用户名
                sqlQuery.bindValue(1,ui->textid->text()); //添加密码 ，
                sqlQuery.exec();
                QMessageBox::information(NULL, "提示", "修改成功！");
                LoginWindow *w=new LoginWindow();
                w->setWindowTitle("登录界面");
                w->show();
                this->close();
            }

        }
        else{
            QMessageBox::information(NULL, "错误", "该用户名不存在！");
        }
    }
    sqlConn.close();
}

void updatewindow::on_pushButton_exit_clicked()
{
    this->close();
}
