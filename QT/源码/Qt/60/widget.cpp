#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_VolumeDown) {
        this->setStyleSheet("QWidget {background-color: red}");
        qDebug() << "音量减键按下" << endl;
    }
    qDebug() << "有按键按下" << endl;
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_VolumeDown) {
        this->setStyleSheet("QWidget {background-color: white}");
        qDebug() << "音量减键松开" << endl;
    }
    qDebug() << "有按键松开" << endl;
}

