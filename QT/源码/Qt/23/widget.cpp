#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 为什么需要设置父对象？因为窗口需要互相联系，比如A需要显示在B上面，A需要指定B为父对象
    pushButton = new QPushButton;
    // 设置父对象的两种方法
    // 1.通过构造函数传参
    // 2.通过setParent()方法
    pushButton->setParent(this);

    // Qt对象树机制，目的就是方便内存管理
    myWidget = new MyWidget();
}

Widget::~Widget()
{
}

