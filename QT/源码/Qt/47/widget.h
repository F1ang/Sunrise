#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QChart> // 图表类
#include <QChartView> // 图表视图类

QT_CHARTS_USE_NAMESPACE // using namespace QT_CHARTS_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
