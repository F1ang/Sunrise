#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 6u/157
    file.setFileName("/sys/class/leds/beep/brightness");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_pressed()
{
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "蜂鸣器不存在！" << endl;
        return;
    }

    QByteArray buf = "1";
    file.write(buf);

    file.close();
}

void Widget::on_pushButton_released()
{
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "蜂鸣器不存在！" << endl;
        return;
    }

    QByteArray buf = "0";
    file.write(buf);

    file.close();
}
