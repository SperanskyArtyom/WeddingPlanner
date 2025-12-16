#include "paymentdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QLocale>

PaymentDialog::PaymentDialog(DatabaseManager *dbManager, const WeddingOrder &order, QWidget *parent)
    : QDialog(parent), m_dbManager(dbManager), m_order(order)
{
    setWindowTitle("Внесение оплаты");
    setMinimumSize(300, 200);

    initUi();
    updateLabels();
}

void PaymentDialog::initUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_budgetLabel = new QLabel(this);
    m_paidLabel   = new QLabel(this);
    m_dueLabel    = new QLabel(this);

    mainLayout->addWidget(m_budgetLabel);
    mainLayout->addWidget(m_paidLabel);
    mainLayout->addWidget(m_dueLabel);

    m_amountEdit = new QDoubleSpinBox(this);
    m_amountEdit->setRange(0, m_order.budget());
    m_amountEdit->setPrefix("₽ ");
    connect(m_amountEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &PaymentDialog::validateAmount);

    mainLayout->addWidget(m_amountEdit);

    m_payButton = new QPushButton("Оплатить", this);
    m_payButton->setEnabled(false);
    connect(m_payButton, &QPushButton::clicked, this, &PaymentDialog::onPayClicked);

    mainLayout->addWidget(m_payButton);
    setLayout(mainLayout);
}

void PaymentDialog::updateLabels()
{
    QLocale ru(QLocale::Russian, QLocale::Russia);
    m_paidAmount = m_dbManager->getPaidAmount(m_order.id());
    double due = m_order.budget() - m_paidAmount;

    m_budgetLabel->setText("Бюджет свадьбы: " + ru.toString(m_order.budget(), 'f', 0) + " ₽");
    m_paidLabel->setText("Оплачено: " + ru.toString(m_paidAmount, 'f', 0) + " ₽");
    m_dueLabel->setText("К оплате: " + ru.toString(due, 'f', 0) + " ₽");

    m_amountEdit->setMaximum(due);
    m_amountEdit->setValue(due > 0 ? due : 0);
}

void PaymentDialog::validateAmount()
{
    m_payButton->setEnabled(m_amountEdit->value() > 0);
}

void PaymentDialog::onPayClicked()
{
    double amount = m_amountEdit->value();
    if (amount <= 0) return;

    if (m_dbManager->addPayment(m_order.id(), amount)) {
        qDebug() << "Оплата добавлена:" << amount;
        updateLabels();
        accept();
    } else {
        qDebug() << "Ошибка при добавлении оплаты";
    }
}
