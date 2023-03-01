#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>

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

    // 重写按键按下的事件
    void keyPressEvent(QKeyEvent *event) override;

    // 重写按键松开事件
    void keyReleaseEvent(QKeyEvent *event) override;

};
#endif // WIDGET_H
