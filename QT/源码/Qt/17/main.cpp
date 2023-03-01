#include "mainwindow.h"

#include <QApplication>

/*
 * 文件命名都是小写字母的
 * 类的名字首写字母是大写的，单词和单词之间首写字母是大写的
 * 除了构造函数和析构函数，成员函数的首写字母都是小写的，单词和单词之间首写字母是大写的
 * 变量的首写字母都是小写的，单词和单词之间首写字母是大写的
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setGeometry(0, 0, 100, 200);
    //    if (1)
    //        return 0;
    //    else
    //        return 1;
    return a.exec();
}
