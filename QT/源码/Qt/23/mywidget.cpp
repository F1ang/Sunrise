#include "mywidget.h"
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{

}

MyWidget::~MyWidget()
{
    qDebug() << "MyWidget的析构函数执行了" << endl;
}
