#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MyWindow; }
QT_END_NAMESPACE

class MyWindow : public QMainWindow
{
    // 宏定义，Qt信号槽需要它
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr);
    ~MyWindow();
    int i;

private:
    Ui::MyWindow *ui;
};
#endif // MYWINDOW_H
