#include "organizermainwindow.h"
#include "ordersearchwidget.h"

OrganizerMainWindow::OrganizerMainWindow(QWidget *parent)
    : MainWindow(parent)
{
    initUi();
    initConnections();
}

void OrganizerMainWindow::initUi()
{
    m_searchWidget = new OrderSearchWidget(this);
    m_orderListWidget = new QListWidget(this);
    m_addOrderButton = new QPushButton("Добавить свадьбу", this);

    mainLayout->addWidget(m_searchWidget);
    mainLayout->addWidget(m_orderListWidget);
    mainLayout->addWidget(m_addOrderButton);
}

void OrganizerMainWindow::initConnections()
{
    connect(m_searchWidget, &OrderSearchWidget::searchChanged,
            this, &OrganizerMainWindow::onSearchWidgetChanged);
}

void OrganizerMainWindow::onSearchWidgetChanged(
    const QString &text,
    OrderSearchWidget::FilterType filter,
    OrderSearchWidget::SortType sort)
{
    qDebug() << "Поиск: " << text;
    qDebug() << "Фильтр: " << static_cast<int>(filter);
    qDebug() << "Сортировка: " << static_cast<int>(sort);
}
