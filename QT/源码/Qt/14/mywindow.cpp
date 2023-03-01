#include "mywindow.h"
#include "ui_mywindow.h"
#include <QDebug>

MyWindow::MyWindow(QWidget *parent)
    : QMainWindow(parent)
    , i(4) // i = 4;
    , ui(new Ui::MyWindow) // ui = new Ui::MyWindow;
{
    //i = 4;
    ui->setupUi(this);
    qDebug() << "构造函数执行了！" << endl;
}

MyWindow::~MyWindow()
{
    qDebug() << "析构函数执行了！" << endl;
    delete ui;
}

