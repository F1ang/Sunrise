#ifndef QQITEM_H
#define QQITEM_H

#include <QWidget>

namespace Ui {
class QQItem;
}

class QQItem : public QWidget
{
    Q_OBJECT

public:
    explicit QQItem(QString icon, bool flag, QString name, QWidget *parent = nullptr);
    ~QQItem();

private:
    Ui::QQItem *ui;
};

#endif // QQITEM_H
