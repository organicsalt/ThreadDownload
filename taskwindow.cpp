#include "taskwindow.h"
#include "ui_taskwindow.h"
#include "widget.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

taskwindow::taskwindow(QString _id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::taskwindow)
{
    ui->setupUi(this);
    id=_id;
}
taskwindow::~taskwindow()
{
    delete ui;
}

void taskwindow::on_pushButton_deleterecord_clicked()
{
    this->close();
}

void taskwindow::on_pushButton_looktask_clicked()
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
    QSqlQuery sqlQuery(sqlConn);
    sqlQuery.prepare(tr("select * from record"));
    if(!sqlQuery.exec()) //执行语句
    {
        qDebug()<<tr("查询失败！").toUtf8().data();
    }
    else
    {
        qDebug()<<tr("查询成功！").toUtf8().data();
        int row=0;
        ui->table->setColumnCount(5);
        while(sqlQuery.next()){
            ui->table->setRowCount(row+1);
            QTableWidgetItem *item0, *item1, *item2, *item3, *item4;
            item0 = new QTableWidgetItem;
            item1 = new QTableWidgetItem;
            item2 = new QTableWidgetItem;
            item3 = new QTableWidgetItem;
            item4 = new QTableWidgetItem;
            item0->setText(sqlQuery.value(0).toString());
            item1->setText(sqlQuery.value(1).toString());
            item2->setText(sqlQuery.value(2).toString());
            item3->setText(sqlQuery.value(3).toString());
            item4->setText(sqlQuery.value(4).toString());
            ui->table->setItem(row,0,item0);
            ui->table->setItem(row,1,item1);
            ui->table->setItem(row,2,item2);
            ui->table->setItem(row,3,item3);
            ui->table->setItem(row,4,item4);
            ui->table->item(row,0)->setTextAlignment(Qt::AlignHCenter);
            ui->table->item(row,1)->setTextAlignment(Qt::AlignHCenter);
            ui->table->item(row,2)->setTextAlignment(Qt::AlignHCenter);
            ui->table->item(row,3)->setTextAlignment(Qt::AlignHCenter);
            ui->table->item(row,4)->setTextAlignment(Qt::AlignHCenter);
            row++;
            qDebug()<<row;
       }

    }
    sqlConn.close();
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
}

void taskwindow::on_table_clicked(const QModelIndex &index)
{
    QList<QTableWidgetItem*> items = ui->table->selectedItems();
    int count = items.count();
    int row = ui->table->row(items.at(0));
    QTableWidgetItem *item = items.at(0);
    this->name=item->text();
    item = items.at(1);
    this->URL=item->text();
    item = items.at(2);
    this->path=item->text();
}

void taskwindow::on_pushButton_openpath_clicked()
{
    if(this->path==""){
        QMessageBox::information(NULL, "错误", "请选择下载任务");
    }
    else{
        bool ok = QDesktopServices::openUrl(QUrl(this->path));
    }
}

void taskwindow::on_pushButton_exit_clicked()
{
    Widget *w_download=new Widget(0,this->URL,this->path,this->name,this->id);
    w_download->setWindowTitle("下载器");
    w_download->show();
}

void taskwindow::on_pushButton_redownload_clicked()
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
    QSqlQuery sqlQuery(sqlConn);
    sqlQuery.prepare(tr("delete from record where name = ?"));
    sqlQuery.bindValue(0,this->name);
    if(!sqlQuery.exec()) //执行语句
    {
        qDebug()<<tr("删除失败！").toUtf8().data();
        QMessageBox::information(NULL, "错误", "请选择任务");
    }
    else{
        QMessageBox::information(NULL, "提示", "删除成功！");
    }
    sqlConn.close();
    taskwindow::on_pushButton_looktask_clicked();
}
