#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 文本的读写流程
    // 1.打开文件》打到文本》QFileDialog::getOpenFileName返回的是
    // 一个字符串，包括了路径+文件名字
    // open()方法打开文本
    // 2.读写文本
    // readAll(),readLine(),write()
    // 3.关闭文本
    // close()
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文本", "/home/alientek");
    qDebug() << fileName << endl;

    // 设置要打开的文件
    file.setFileName(fileName);

    // 打开文件
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "文本打开失败" << endl;
        return;
    }

    // QString的构造函数自动转换
    ui->textEdit->setPlainText(file.readAll());

    // 关闭文件
    file.close();
}

void Widget::on_pushButton_2_clicked()
{
    if (file.fileName().isEmpty())
        return;

    // 打开文件
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "文本打开失败" << endl;
        return;
    }

    // 写文件
    file.write(ui->textEdit->toPlainText().toUtf8());

    // 关闭
    file.close();

    ui->textEdit->clear();
}
