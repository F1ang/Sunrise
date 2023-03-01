#include "mainwindow.h"

#include <QApplication>

/*
 * 新建项目，ctrl + n
 * 运行项目，ctrl + r
 * 构建项目，ctrl + b
 * 改变编辑器界面字体显示比例大小，ctrl + 鼠标滚轮
 * 对齐代码，ctrl + a; ctrl + i
 * 跳转到上一行，ctrl + shift + enter
 * 跳转到下一行，ctrl + enter
 * 向上移动行，ctrl + shift + up
 * 向下移动行，ctrl + shift + down
 * 复制当前行到上一行，ctrl + shift + pgup
 * 复制当前行到下一行，ctrl + shift + pgdown
 * 头文件和源文件切换，F4
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
