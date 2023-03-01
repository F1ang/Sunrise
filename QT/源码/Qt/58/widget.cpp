#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 开发板的led接口，157/6u
    file.setFileName("/sys/class/leds/sys-led/brightness");
    // 默认是出厂系统的 LED 心跳的触发方式,想要控制 LED，
    // 需要改变 LED 的触发方式，改为 none，即无
    system("echo none > /sys/class/leds/sys-led/trigger");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked(bool checked)
{
    qDebug() << checked << endl;
    if (!file.exists()) {
        qDebug() << "LED不存在！" << endl;
        return;
    }

    if (!file.open(QIODevice::WriteOnly))
        return;

    QByteArray buf[] = {"0", "1"};

    if (checked) {
        ui->pushButton->setText("关");
        // 开
        file.write(buf[1]);
    } else {
        ui->pushButton->setText("开");
        // 关
        file.write(buf[0]);
    }

    file.close();
}
