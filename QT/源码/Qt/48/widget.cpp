#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsColorizeEffect> // QGraphicsEffect
#include <QGraphicsOpacityEffect>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 几何属性动画初始化
    propertyAnimation  = new QPropertyAnimation(ui->geometryWidget, "geometry");

    // 设置动画起始值
    propertyAnimation->setStartValue(QRect(0, 0, 100, 100));
    // 设置某个时间段值
    propertyAnimation->setKeyValueAt(0.2, QRect(600, 0, 100, 100));
    // 设置结束值
    propertyAnimation->setEndValue(QRect(600, 400, 200, 200));
    // 设置动画时长
    propertyAnimation->setDuration(2000);
    // 设置动画循环周期
    propertyAnimation->setLoopCount(1);
    // 设置动画的缓和曲线（动画曲线）
    propertyAnimation->setEasingCurve(QEasingCurve::OutQuart);

    // 颜色动画
    QGraphicsColorizeEffect *graphicsColorizeEffect = new QGraphicsColorizeEffect(this);
    ui->colorWidget->setGraphicsEffect(graphicsColorizeEffect);
    propertyAnimation1 = new QPropertyAnimation(graphicsColorizeEffect, "color");
    propertyAnimation1->setStartValue(QColor(Qt::yellow)); // rgb(100, 100, 100);
    propertyAnimation1->setEndValue(QColor(Qt::darkGray)); // rgb(20, 20, 50)
    propertyAnimation1->setDuration(2000);

    // 不透明度动画
    QGraphicsOpacityEffect *graphicsOpacityEffect = new QGraphicsOpacityEffect(this);
    ui->opacityWidget->setGraphicsEffect(graphicsOpacityEffect);
    propertyAnimation2 = new QPropertyAnimation(graphicsOpacityEffect, "opacity");
    propertyAnimation2->setStartValue(0.0);
    propertyAnimation2->setEndValue(1.0);
    propertyAnimation2->setDuration(5000);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    propertyAnimation->start();
}

void Widget::on_pushButton_2_clicked()
{
    propertyAnimation1->start();
}

void Widget::on_pushButton_3_clicked()
{
    propertyAnimation2->start();
}
