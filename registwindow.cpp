#include "registwindow.h"
#include "ui_registwindow.h"
#include "loginwindow.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

registwindow::registwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registwindow)
{
    ui->setupUi(this);
    ui->textpassword->setEchoMode(QLineEdit::Password);
}

registwindow::~registwindow()
{
    delete ui;
}

void registwindow::on_pushButton_register_clicked()
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
    sqlQuery.prepare(tr("select * from user where id = ?"));
    sqlQuery.addBindValue(ui->textid->text()); //添加用户名
    if(!sqlQuery.exec()) //执行语句
    {
        qDebug()<<tr("查询失败！").toUtf8().data();
    }
    else
    {
        if(sqlQuery.first()){
            //如果有数据，循环读取，把数据显示出来
            QMessageBox::information(NULL, "错误", "该用户名已存在！");
        }
        else{
            sqlQuery.prepare(tr("insert into user(id,password) values(?,?)"));
            sqlQuery.bindValue(0,ui->textid->text()); //添加用户名
            sqlQuery.bindValue(1,ui->textpassword->text()); //添加密码 ，
            sqlQuery.exec();
            QMessageBox::information(NULL, "提示", "注册成功！");
            LoginWindow *w=new LoginWindow();
            w->setWindowTitle("登录窗口");
            w->show();
            this->close();
        }
    }
    sqlConn.close();
}

void registwindow::on_pushButton_exit_clicked()
{
    this->close();
}
