#include "usermainwindow.h"

#include "paymentdialog.h"

UserMainWindow::UserMainWindow(QWidget *parent)
    : MainWindow(parent)
{
    initUi();
    initConnections();

    m_order = m_dbManager->getOrder(1);
    m_orderCard->setOrder(m_order);
}

void UserMainWindow::initUi()
{
    m_orderCard = new ClientOrderCard(this);
    m_payButton = new QPushButton("Оплатить", this);

    mainLayout->addWidget(m_orderCard);
    mainLayout->addStretch();
    mainLayout->addWidget(m_payButton);
}

void UserMainWindow::initConnections()
{
    connect(m_payButton, &QPushButton::clicked,
            this, &UserMainWindow::onPayButtonClicked);
}

void UserMainWindow::onPayButtonClicked()
{
    if (!m_dbManager || m_order.id() == -1)
        return;

    PaymentDialog dialog(m_dbManager, m_order, this);
    dialog.exec();
}
