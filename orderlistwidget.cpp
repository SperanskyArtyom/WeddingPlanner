#include "orderlistwidget.h"
#include <QVBoxLayout>
#include <QListWidgetItem>

OrderListWidget::OrderListWidget(QWidget *parent)
    : QWidget(parent)
{
    m_listWidget = new QListWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_listWidget);
    setLayout(layout);

    connect(m_listWidget, &QListWidget::itemDoubleClicked,
            this, &OrderListWidget::onItemDoubleClicked);
}

void OrderListWidget::setOrders(const QList<WeddingOrder> &orders)
{
    m_orders = orders;
    m_listWidget->clear();

    for (const auto &order : orders)
    {
        QString text = QString("%1\nДата: %2\nБюджет: %3 рублей")
                           .arg(order.clientName())
                           .arg(order.date().toString("dd.MM.yy"))
                           .arg(QString::number(order.budget(), 'f', 0));
        QListWidgetItem *item = new QListWidgetItem(text, m_listWidget);
        item->setData(Qt::UserRole, order.id());
    }
}

WeddingOrder OrderListWidget::selectedOrder() const
{
    auto item = m_listWidget->currentItem();
    if (!item)
        return WeddingOrder();

    int id = item->data(Qt::UserRole).toInt();
    for (const auto &order : m_orders)
    {
        if (order.id() == id)
            return order;
    }

    return WeddingOrder();
}

void OrderListWidget::onItemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item)
    emit orderSelected(selectedOrder());
}
