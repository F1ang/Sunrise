#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

#include <QDebug>
#include <QFontMetrics>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , offset(0)
    , textContent("正点原子Linux开发板")
{
    ui->setupUi(this);

    timer = new QTimer(this);

    // 定时x毫秒
    timer->start(20);

    font.setPixelSize(50);

    // 显示的文字宽度计算
    QFontMetrics fontMetrics(font);
    textContentWidth = fontMetrics.width(textContent);

    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    QPen pen;
    pen.setColor(QColor(Qt::red));

    painter.setPen(pen);

    painter.setFont(font);

    QRectF rectF = this->rect();

    // offset偏移
    rectF.setLeft(this->rect().width() - offset);

    qDebug() << rectF.width() << endl;

    painter.drawText(rectF, Qt::AlignVCenter, textContent);

}

void Widget::onTimerTimeOut()
{
    if (offset < this->width() + textContentWidth)
        offset += 1;
    else
        offset = 0;

    // 特别重要，更新界面
    this->update();
}

