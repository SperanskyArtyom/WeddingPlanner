#include "usermainwindow.h"

UserMainWindow::UserMainWindow(QWidget *parent)
    : MainWindow(parent)
{
    initUi();
    initConnections();
}

void UserMainWindow::initUi()
{
    m_orderLabel = new QLabel("Информация о заказе", this);
    m_payButton = new QPushButton("Оплатить", this);

    mainLayout->addWidget(m_orderLabel);
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
    qDebug() << "Открываем виджет оплаты";
}
