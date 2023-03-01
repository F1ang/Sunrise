#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

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

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    // this是指定给图的对象
    QPainter painter(this);

    // 设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setWidth(5);
    //pen.setColor(QColor("#888888"));
    pen.setColor(QColor(200, 100, 50));

    QBrush brush(QColor(200, 100, 50));
    ///* brush.setColor(QColor(200, 100, 50));*/

    // 将画刷给画家
    // painter.setBrush(brush);

    // 将画笔给画家
    painter.setPen(pen);

    // 画矩形
    painter.drawRect(200, 100, 100, 100);

    // 多边形
    QPolygon polygon;
    polygon.setPoints(3, 100, 20, 200, 50, 300, 300);

    // 画三角形
    painter.drawPolygon(polygon);

    // 画直线
    painter.drawLine(400, 400, 500, 500);

    // 画椭圆
    painter.drawEllipse(200, 200, 50, 100);

    // 画文字
    QRectF rectF(0, 0, 200, 100);
    painter.drawText(rectF, Qt::AlignHCenter, "正点原子");

    // 画路径
    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    painter.drawPath(path);
}


