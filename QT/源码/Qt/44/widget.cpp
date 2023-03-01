#include "widget.h"
#include "ui_widget.h"
#include "filedialog.h"
#include <QFile>
#include <QDateTime>

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


void Widget::on_pushButton_clicked()
{
    QFile file;
    file.setFileName(QDateTime::currentDateTime().toString("MMddhhmmss") + ".txt");

    file.open(QIODevice::ReadWrite);

    FileDialog *fileDialog = new FileDialog(this);
    fileDialog->resize(this->size());
    fileDialog->show();
    fileDialog->setModal(true);
    fileDialog->exec();

    QString tmp = fileDialog->getTextEditContent();
    file.write(tmp.toUtf8());
    file.close();

    if (tmp.length() == 0)
        file.remove();

    delete fileDialog;
}
