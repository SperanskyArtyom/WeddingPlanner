#include "organizermainwindow.h"
#include "ordersearchwidget.h"

OrganizerMainWindow::OrganizerMainWindow(QWidget *parent)
    : MainWindow(parent)
{
    initUi();
    initConnections();

    updateOrderList();
}

void OrganizerMainWindow::initUi()
{
    m_searchWidget = new OrderSearchWidget(this);
    m_orderListWidget = new OrderListWidget(this);
    m_addOrderButton = new QPushButton("Добавить свадьбу", this);

    mainLayout->addWidget(m_searchWidget);
    mainLayout->addWidget(m_orderListWidget);
    mainLayout->addWidget(m_addOrderButton);
}

void OrganizerMainWindow::initConnections()
{
    connect(m_searchWidget, &OrderSearchWidget::searchChanged,
            this, &OrganizerMainWindow::updateOrderList);
    connect(m_orderListWidget, &OrderListWidget::orderSelected,
            this, &OrganizerMainWindow::onOrderSelected);
}

void OrganizerMainWindow::onOrderSelected()
{
    qDebug() << "item selected";
}

void OrganizerMainWindow::updateOrderList()
{
    if (!m_dbManager || !m_searchWidget)
        return;

    auto filter = static_cast<DatabaseManager::OrderFilter>(m_searchWidget->filterType());
    auto sort   = static_cast<DatabaseManager::OrderSort>(m_searchWidget->sortType());
    QString text = m_searchWidget->searchText();

    QList<WeddingOrder> orders = m_dbManager->getOrders(filter, sort, text);
    m_orderListWidget->setOrders(orders);
}
