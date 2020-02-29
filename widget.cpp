#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

Widget::Widget(int type, QString url, QString saveFile,QString fileName,QString id, QWidget *parent) :
    QWidget(parent),type(type),url(url),saveFile(saveFile),id(id),filename(fileName)
{
    control = new DownloadControl(this);
    connect(control,SIGNAL(finished(int)),SLOT(finishedSlot(int)));
    connect(control,SIGNAL(progressChange(int,qint64,qint64,qint64)),SLOT(progressChangeSlot(int,qint64,qint64,qint64)));
    connect(control,SIGNAL(error(int,QString)),SLOT(errorSlot(int,QString)));

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),SLOT(speedSlot()));

    initWidget();
}

Widget::~Widget()
{
}

void Widget::initWidget()
{
    setWindowTitle(title);
    resize(500,400);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    fileName = new QLabel("未知");
    mainLayout->addWidget(fileName);
    errorLabel = new QLabel("一切正常");
    mainLayout->addWidget(errorLabel);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(new QLabel("线程数："));
    countSpinBox = new QSpinBox;
    layout->addWidget(countSpinBox);
    layout->addSpacerItem(new QSpacerItem(20,2,QSizePolicy::Preferred,QSizePolicy::Minimum));

    layout->addWidget(new QLabel("正在运行的线程数："));
    runningCountSpinBox = new QSpinBox;
    layout->addWidget(runningCountSpinBox);
    layout->addSpacerItem(new QSpacerItem(20,2,QSizePolicy::Expanding,QSizePolicy::Minimum));
    mainLayout->addLayout(layout);

    layout = new QHBoxLayout;
    layout->addSpacerItem(new QSpacerItem(20,2,QSizePolicy::Expanding,QSizePolicy::Minimum));
    startBtn = new QPushButton;
    layout->addWidget(startBtn);
    layout->addSpacerItem(new QSpacerItem(20,2,QSizePolicy::Preferred,QSizePolicy::Minimum));
    pauseBtn = new QPushButton;
    layout->addWidget(pauseBtn);
    layout->addSpacerItem(new QSpacerItem(20,2,QSizePolicy::Preferred,QSizePolicy::Minimum));
    restartBtn = new QPushButton;
    layout->addWidget(restartBtn);
    layout->addSpacerItem(new QSpacerItem(20,2,QSizePolicy::Preferred,QSizePolicy::Minimum));
    stopBtn = new QPushButton;
    layout->addWidget(stopBtn);
    layout->addSpacerItem(new QSpacerItem(20,2,QSizePolicy::Expanding,QSizePolicy::Minimum));
    mainLayout->addLayout(layout);

    listWidget = new QListWidget;
    mainLayout->addWidget(listWidget);

    setLayout(mainLayout);

    countSpinBox->setValue(4);
    runningCountSpinBox->setValue(0);
    countSpinBox->setEnabled(type == 0);
    runningCountSpinBox->setEnabled(false);

    fileName->setFont(QFont("楷体",23));
    errorLabel->setFont(QFont("宋体",7));

    listWidget->setResizeMode(QListView::Adjust);
    listWidget->setAutoScroll(true);

    startBtn->setText("开始");
    pauseBtn->setText("暂停");
    restartBtn->setText("继续");
    stopBtn->setText("停止");
    connect(startBtn,SIGNAL(clicked()),SLOT(start()));
    connect(pauseBtn,SIGNAL(clicked()),SLOT(pause()));
    connect(restartBtn,SIGNAL(clicked()),SLOT(restart()));
    connect(stopBtn,SIGNAL(clicked()),SLOT(stop()));
    pauseBtn->setEnabled(false);
    restartBtn->setEnabled(false);
    stopBtn->setEnabled(false);
}

void Widget::start()
{
    countSpinBox->setEnabled(false);
    startBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    stopBtn->setEnabled(true);

    int count = countSpinBox->value();
    if(type)count = control->getSettingCount(url);

    if(count >= 1 && count <= 15)
    {
        widgets.clear();
        listWidget->clear();
        for(int i = 0;i <= count;i ++)
        {
            DownloadWidgets *widget = new DownloadWidgets(listWidget);
            widgets.append(widget);
            QListWidgetItem *item = new QListWidgetItem(listWidget);
            listWidget->setItemWidget(item,widget);
            item->setSizeHint(widget->size());
        }
    }

    if(type == 0)
    {
        if(!control->downloadFile(url,saveFile+filename,count))
            return;
    }else{
        if(!control->downloadFile(url))
            return;
    }

    countSpinBox->setValue(control->getCount());
    runningCountSpinBox->setValue(control->getRunningCount());

    QFileInfo info(control->getSaveFile());
    fileName->setText(info.fileName());

    timer->start();
}

void Widget::pause()
{
    control->pause();
    timer->stop();
    runningCountSpinBox->setValue(control->getRunningCount());

    pauseBtn->setEnabled(false);
    restartBtn->setEnabled(true);
}

void Widget::restart()
{
    control->restart();
    timer->start();
    runningCountSpinBox->setValue(control->getRunningCount());

    pauseBtn->setEnabled(true);
    restartBtn->setEnabled(false);
}

void Widget::stop()
{
    control->stop();
    timer->stop();
    runningCountSpinBox->setValue(control->getRunningCount());

    pauseBtn->setEnabled(false);
    restartBtn->setEnabled(false);
    stopBtn->setEnabled(false);
}

void Widget::finishedSlot(int index)
{
    qDebug()<<"finish : "<<index;
    runningCountSpinBox->setValue(control->getRunningCount());
    if(index == 0 && control->getState() == DownloadControl::Finished)
    {
        pauseBtn->setEnabled(false);
        restartBtn->setEnabled(false);
        stopBtn->setEnabled(false);
        qDebug()<<"finished";
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
        sqlQuery.prepare(tr("select * from record where name = ?"));
        sqlQuery.addBindValue(this->filename);
        if(!sqlQuery.exec()) //执行语句
        {
            qDebug()<<tr("查询失败！").toUtf8().data();
        }
        else
        {
            if(sqlQuery.first()){
                //如果有数据，循环读取，把数据显示出来
                QMessageBox::information(NULL, "错误", "该文件已存在！");
            }
            else{
                sqlQuery.prepare(tr("insert into record(name,link,path,time,id) values(?,?,?,?,?)"));
                sqlQuery.bindValue(0,this->filename);
                sqlQuery.bindValue(1,this->url);
                sqlQuery.bindValue(2,this->saveFile);
                QDateTime time = QDateTime::currentDateTime();
                sqlQuery.bindValue(3,time);
                sqlQuery.bindValue(4,this->id);
                sqlQuery.exec();
                QMessageBox::information(NULL, "提示", "下载完成！");
                this->close();
            }
        }
        sqlConn.close();
    }
}

void Widget::progressChangeSlot(int index, qint64 startPoint, qint64 endPoint, qint64 readySize)
{
    DownloadWidgets *widget = widgets.at(index);
    widget->updateReady(startPoint,endPoint,readySize);
}

void Widget::errorSlot(int index, QString errorString)
{
    errorLabel->setText("来自 ： " + QString::number(index) + " \n" + errorString);
}

void Widget::speedSlot()
{
    foreach(DownloadWidgets *widget,widgets)
        widget->updateSpeed();
    if(control->getRunningCount() == 0)
        timer->stop();
}
