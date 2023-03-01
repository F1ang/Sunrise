#include "mywindow.h"

#include <QApplication>

// 程序的入口
int main(int argc, char *argv[])
{
    // argc是命令行参数个数，argv是命令行参数
    // QApplication a(argc, argv),管理Qt程序的运行，和设置Qt应用程序，针对QWidget应用程序
    // QGuiApplication a(argc, argv),管理Qt程序的运行，和设置Qt应用程序，针对非QWidget应用程序，比如QQuick
    // QCoreApplication a(argc, argv),管理Qt程序的运行，和设置Qt应用程序，针对无界面的应用程序
    QApplication a(argc, argv);
    // MyWindow是我们自己定义的类，w是创建的对象
    MyWindow w;
    // w对象调用了show()方法
    w.show();
    // 事件循环，QEventLoop::exec()，等待鼠标或者键盘等其他的输入
    return a.exec();
}
