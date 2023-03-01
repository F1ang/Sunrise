#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <QDebug>

class MyThread;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    MyThread *myThread;
};

class MyThread : public QThread
{
    Q_OBJECT

public:
    MyThread(QWidget *parent = nullptr) {
        Q_UNUSED(parent)
    }
    ~MyThread() {
     qDebug() << "线程销毁" << endl;
    }
    // 只有这个run()方法在新的线程里面
    void run() override {
        qDebug() << "线程开启" << endl;
        sleep(5);
        qDebug() << "线程结束" << endl;
        deleteLater();
    }
};

#endif // WIDGET_H
