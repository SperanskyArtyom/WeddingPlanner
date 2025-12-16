#ifndef ORDERLISTWIDGET_H
#define ORDERLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "weddingorder.h"

class OrderListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OrderListWidget(QWidget *parent = nullptr);

    void setOrders(const QList<WeddingOrder> &orders);

    WeddingOrder selectedOrder() const;

signals:
    void orderSelected(const WeddingOrder &order);

private slots:
    void onItemDoubleClicked(QListWidgetItem *item);

private:
    QListWidget *m_listWidget;
    QList<WeddingOrder> m_orders;
};

#endif // ORDERLISTWIDGET_H
