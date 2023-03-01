#include "filedialog.h"
#include "ui_filedialog.h"

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);
}

FileDialog::~FileDialog()
{
    delete ui;
}

QString FileDialog::getTextEditContent()
{
    return  ui->textEdit->toPlainText();
}

void FileDialog::on_pushButton_pressed()
{
    ui->pushButton->setIcon(QIcon(":/icons/back_press.png"));
}

void FileDialog::on_pushButton_released()
{
    ui->pushButton->setIcon(QIcon(":/icons/back.png"));
}

void FileDialog::on_pushButton_clicked()
{
    this->close();
}
