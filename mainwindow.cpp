#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskwindow.h"
#include <QDebug>
#include <QFileDialog>
#include "widget.h"
#include "downloadcontrol.h"

mainwindow::mainwindow(QString _id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    id=_id;
}

mainwindow::~mainwindow()
{
    delete ui;
}


void mainwindow::on_pushButton_download_clicked()
{
    QString URL = ui->textURL->text();
    QString fileName = ui->textfilename->text();
    QString saveFile = ui->textpath->text();
    //DownloadControl *control = new DownloadControl(this);
    qDebug()<<URL;
    qDebug()<<saveFile;
    /*
    if(!control->downloadFile(URL,saveFile,5)){
        return ;
    }
    */
    Widget *w_download=new Widget(0,URL,saveFile,fileName,id);
    w_download->setWindowTitle("下载器");
    w_download->show();
}

void mainwindow::on_pushButton_choose_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("选中文件夹"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("File(*.*)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames=fileDialog->selectedFiles();
    }
    QString filename = fileNames.join("/");
    filename=filename.append("/");
    ui->textpath->setText(filename);
}

void mainwindow::on_pushButton_exit_clicked()
{
    this->close();
}

void mainwindow::on_pushButton_display_clicked()
{
    taskwindow *w=new taskwindow(this->id);
    w->setWindowTitle("任务列表");
    w->show();
}
