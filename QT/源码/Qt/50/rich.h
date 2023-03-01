#ifndef RICH_H
#define RICH_H

#include <QWidget>

namespace Ui {
class Rich;
}

// 富人类，有钱人类
class Rich : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal money READ money WRITE setMoney)

public:
    explicit Rich(QWidget *parent = nullptr);
    ~Rich();
    qreal money() const;
    void setMoney(qreal m);

private:
    Ui::Rich *ui;
    qreal richMoney;
};

#endif // RICH_H
