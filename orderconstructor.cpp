#include "orderconstructor.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

OrderConstructor::OrderConstructor(DatabaseManager *dbManager, QWidget *parent)
    : QDialog(parent), m_dbManager(dbManager)
{
    setWindowTitle("Конструктор заказа");
    resize(400, 480);
    setMinimumSize(300, 480);

    initUi();
}

void OrderConstructor::initUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel("Имя клиента:", this);
    m_clientNameEdit = new QLineEdit(this);
    connect(m_clientNameEdit, &QLineEdit::textChanged, this, &OrderConstructor::validateInputs);

    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(m_clientNameEdit);

    QLabel *dateLabel = new QLabel("Дата:", this);
    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    connect(m_dateEdit, &QDateEdit::dateChanged,
            this, &OrderConstructor::validateInputs);
    connect(m_dateEdit, &QDateEdit::dateChanged,
            this, &OrderConstructor::onDateChanged);

    mainLayout->addWidget(dateLabel);
    mainLayout->addWidget(m_dateEdit);

    QLabel *budgetLabel = new QLabel("Бюджет:", this);
    m_budgetEdit = new QDoubleSpinBox(this);
    m_budgetEdit->setRange(0, 1e9);
    m_budgetEdit->setPrefix("₽ ");
    connect(m_budgetEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OrderConstructor::validateInputs);

    mainLayout->addWidget(budgetLabel);
    mainLayout->addWidget(m_budgetEdit);

    QLabel *servicesLabel = new QLabel("Услуги:", this);
    m_servicesLayout = new QVBoxLayout();
    mainLayout->addWidget(servicesLabel);
    mainLayout->addLayout(m_servicesLayout);

    for (int i = 0; i < 4; ++i) {
        initServiceRow(static_cast<Service::Type>(i));
    }

    QLabel *commentLabel = new QLabel("Комментарий:", this);
    m_commentEdit = new QTextEdit(this);
    mainLayout->addWidget(commentLabel);
    mainLayout->addWidget(m_commentEdit);

    m_addButton = new QPushButton("Добавить", this);
    m_addButton->setEnabled(false);
    connect(m_addButton, &QPushButton::clicked, this, &OrderConstructor::onAddClicked);

    mainLayout->addStretch();
    mainLayout->addWidget(m_addButton);
    setLayout(mainLayout);

    onDateChanged(m_dateEdit->date());
}

void OrderConstructor::initServiceRow(Service::Type type)
{
    QString typeName;
    switch (type) {
    case Service::Type::Photo: typeName = "Фотограф"; break;
    case Service::Type::Host: typeName = "Ведущий"; break;
    case Service::Type::Decor: typeName = "Декор"; break;
    case Service::Type::Transport: typeName = "Транспорт"; break;
    }

    QHBoxLayout *rowLayout = new QHBoxLayout();
    QLabel *label = new QLabel(typeName + ":", this);
    QComboBox *combo = new QComboBox(this);
    combo->addItem("не назначено");

    rowLayout->addWidget(label);
    rowLayout->addWidget(combo);
    m_servicesLayout->addLayout(rowLayout);

    m_serviceCombos[type] = combo;
}

void OrderConstructor::validateInputs()
{
    bool valid = !m_clientNameEdit->text().trimmed().isEmpty()
    && m_budgetEdit->value() > 0
        && m_dateEdit->date().isValid();
    m_addButton->setEnabled(valid);
}

void OrderConstructor::onAddClicked()
{
    WeddingOrder order;
    order.setClientName(m_clientNameEdit->text().trimmed());
    order.setDate(m_dateEdit->date());
    order.setBudget(m_budgetEdit->value());
    order.setComment(m_commentEdit->toPlainText().trimmed());

    int orderId = m_dbManager->addOrder(order);
    if (orderId == -1)
        return;

    for (int i = 0; i < 4; ++i) {
        Service::Type type = static_cast<Service::Type>(i);
        QString performer = m_serviceCombos[type]->currentText();
        if (performer == "не назначено")
            continue;

        Service service;
        service.setType(type);
        service.setPerformerName(performer);

        if (!m_dbManager->saveService(orderId, service))
        {
            qDebug() << "service save error";
        }
    }

    emit orderAdded(order);
    accept();
}

void OrderConstructor::updateAvailablePerformers(Service::Type type)
{
    QComboBox *combo = m_serviceCombos.value(type, nullptr);
    if (!combo || !m_dbManager)
        return;

    QString currentSelection = combo->currentText();

    combo->blockSignals(true);
    combo->clear();
    combo->addItem("не назначено");

    QDate date = m_dateEdit->date();

    auto performers = m_dbManager->getAvailablePerformers(type, date);

    for (const QString &name : performers)
        combo->addItem(name);

    int index = combo->findText(currentSelection);
    if (index >= 0)
        combo->setCurrentIndex(index);

    combo->blockSignals(false);
}

void OrderConstructor::onDateChanged(const QDate &date)
{
    Q_UNUSED(date);
    for (int i = 0; i < 4; i++) {
        updateAvailablePerformers(static_cast<Service::Type>(i));
    }
}
