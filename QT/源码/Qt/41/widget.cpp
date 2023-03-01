#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 添加项
    ui->listWidget->addItem("张三");
    ui->listWidget->addItem("李四");
    ui->listWidget->addItem("王五");

    // 移除项
    ui->listWidget->takeItem(0);

    // 插入项，从项前面插入
    ui->listWidget->insertItem(0, "张三");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_listWidget_currentRowChanged(int currentRow)
{
    qDebug() << "下标是：" << currentRow << "项的内容是："  << ui->listWidget->item(currentRow)->text() << endl;
}
