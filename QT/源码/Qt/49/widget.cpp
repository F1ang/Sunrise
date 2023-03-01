#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    rich = new Rich(this);

    // 会自动调用WRITE的方法
    rich->setProperty("money", 200);
    qDebug() <<  "富人有"<< rich->money() << "万" << endl;
}

Widget::~Widget()
{
    delete ui;
}

