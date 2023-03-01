#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 一个应用程序至少要一个进程， QProcess
    // 一个进程至少会有一个线程, QThread
    //myThread = new MyThread(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    // system("sleep 5");
    // system("ping 192.168.1.1");
    // 启动线程
//    myThread->start();

     MyThread *testThread = new MyThread(this);
     testThread->start();
}

void Widget::on_pushButton_2_clicked()
{
    // 终止线程
//    if (!myThread->isFinished())
//        myThread->terminate();
}
