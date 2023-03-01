#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // 设置无边框
    setWindowFlag(Qt::FramelessWindowHint);
    // 设置背景透明
    setAttribute(Qt::WA_TranslucentBackground);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_clicked()
{
    // TODO
    this->close();
}

void Dialog::on_pushButton_2_clicked()
{
    // TODO
    this->close();
}
