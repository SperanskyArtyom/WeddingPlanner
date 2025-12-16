#include "clientordercard.h"

ClientOrderCard::ClientOrderCard(QWidget *parent)
    : QWidget(parent)
{
    m_titleLabel = new QLabel("<b>Моя свадьба</b>", this);
    m_dateLabel = new QLabel(this);
    m_budgetLabel = new QLabel(this);
    m_photographerLabel = new QLabel(this);
    m_hostLabel = new QLabel(this);
    m_decorLabel = new QLabel(this);
    m_transportLabel = new QLabel(this);

    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_titleLabel);
    m_layout->addWidget(m_dateLabel);
    m_layout->addWidget(m_budgetLabel);
    m_layout->addWidget(m_photographerLabel);
    m_layout->addWidget(m_hostLabel);
    m_layout->addWidget(m_decorLabel);
    m_layout->addWidget(m_transportLabel);
    m_layout->addStretch();

    setLayout(m_layout);
}

void ClientOrderCard::setOrder(const WeddingOrder &order)
{
    m_dateLabel->setText("Дата: " + order.date().toString("dd.MM.yy"));
    m_budgetLabel->setText("Бюджет: " + QString::number(order.budget()) + " рублей");

    auto getPerformerText = [](const QString &name) {
        return name.isEmpty() ? "не назначено" : name;
    };

    m_photographerLabel->setText("Фотограф: " + getPerformerText(order.servicePerformer(Service::Type::Photo)));
    m_hostLabel->setText("Ведущий: " + getPerformerText(order.servicePerformer(Service::Type::Host)));
    m_decorLabel->setText("Декор: " + getPerformerText(order.servicePerformer(Service::Type::Decor)));
    m_transportLabel->setText("Транспорт: " + getPerformerText(order.servicePerformer(Service::Type::Transport)));
}
