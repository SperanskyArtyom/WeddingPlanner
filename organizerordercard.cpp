#include "organizerordercard.h"

#include <QDebug>
#include "paymentdialog.h"

OrganizerOrderCard::OrganizerOrderCard(DatabaseManager *dbManager, QWidget *parent)
    : QWidget(parent), m_dbManager(dbManager)
{
    initUi();
}

void OrganizerOrderCard::initUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    m_clientNameLabel = new QLabel(leftWidget);
    m_dateLabel = new QLabel(leftWidget);
    m_budgetLabel = new QLabel(leftWidget);

    leftLayout->addWidget(m_clientNameLabel);
    leftLayout->addWidget(m_dateLabel);
    leftLayout->addWidget(m_budgetLabel);

    m_servicesLayout = new QVBoxLayout();
    leftLayout->addLayout(m_servicesLayout);

    m_paymentButton = new QPushButton("Внести оплату", leftWidget);
    leftLayout->addStretch();
    leftLayout->addWidget(m_paymentButton);

    connect(m_paymentButton, &QPushButton::clicked,
            this, &OrganizerOrderCard::onPaymentClicked);

    leftWidget->setLayout(leftLayout);

    m_taskListWidget = new TaskListWidget(m_dbManager, -1, this);
    m_taskListWidget->setMaximumWidth(300);

    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(m_taskListWidget);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 1);

    setLayout(mainLayout);
}

void OrganizerOrderCard::setOrder(const WeddingOrder &order)
{
    m_order = order;

    m_clientNameLabel->setText("Имя клиента: " + order.clientName());
    m_dateLabel->setText("Дата: " + order.date().toString("dd.MM.yy"));
    m_budgetLabel->setText("Бюджет: " + QString::number(order.budget(), 'f', 0) + " рублей");

    QLayoutItem *child;
    while ((child = m_servicesLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    m_serviceCombos.clear();

    for (int i = 0; i < 4; ++i) {
        Service::Type type = static_cast<Service::Type>(i);
        QString performer = order.servicePerformer(type);
        initServiceRow(type, performer);
    }

    if (m_taskListWidget)
        m_taskListWidget->deleteLater();

    m_taskListWidget = new TaskListWidget(m_dbManager, order.id(), this);
    m_taskListWidget->setMinimumWidth(300);
    layout()->addWidget(m_taskListWidget);
}

void OrganizerOrderCard::initServiceRow(Service::Type type, const QString &performerName)
{
    QHBoxLayout *rowLayout = new QHBoxLayout();
    QLabel *label = new QLabel(this);
    QString typeName;
    switch (type) {
    case Service::Type::Photo: typeName = "Фотограф"; break;
    case Service::Type::Host: typeName = "Ведущий"; break;
    case Service::Type::Decor: typeName = "Декор"; break;
    case Service::Type::Transport: typeName = "Транспорт"; break;
    }
    label->setText(typeName + ":");

    QComboBox *combo = new QComboBox(this);
    combo->addItem(performerName.isEmpty() ? "не назначено" : performerName);
    combo->addItems(m_dbManager->getAvailablePerformers(type, m_order.date()));

    connect(combo, &QComboBox::currentTextChanged, [this, type](const QString &text){
        onPerformerChanged(type, text);
    });

    rowLayout->addWidget(label);
    rowLayout->addWidget(combo);
    m_servicesLayout->addLayout(rowLayout);

    m_serviceCombos[type] = combo;
}

void OrganizerOrderCard::onPerformerChanged(Service::Type type, const QString &performerName)
{
    Service service = m_order.service(type);

    if (performerName == "не назначено")
        service.setPerformerName("");
    else
        service.setPerformerName(performerName);

    if (!m_dbManager->saveService(m_order.id(), service))
    {
        qDebug() << "Ощибка сохранения услуги";
        return;
    }
    qDebug() << "Изменён исполнитель услуги" << static_cast<int>(type) << ":" << performerName;
}

void OrganizerOrderCard::onPaymentClicked()
{
    if (!m_dbManager || m_order.id() == -1)
        return;

    PaymentDialog dialog(m_dbManager, m_order, this);
    dialog.exec();
}
