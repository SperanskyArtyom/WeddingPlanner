#include "ordersearchwidget.h"

#include <QHBoxLayout>

OrderSearchWidget::OrderSearchWidget(QWidget *parent)
    : QWidget(parent)
{
    initUi();
    initFilterMenu();

    connect(m_searchLineEdit, &QLineEdit::textChanged,
            this, &OrderSearchWidget::onSearchTextChanged);
}

void OrderSearchWidget::initUi()
{
    m_searchLineEdit = new QLineEdit(this);
    m_searchLineEdit->setPlaceholderText("Поиск...");

    m_filterButton = new QPushButton("Фильтр", this);
    m_filterButton->setFixedWidth(100);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_searchLineEdit);
    layout->addWidget(m_filterButton);
    layout->setContentsMargins(0, 0, 0, 0);
}

void OrderSearchWidget::initFilterMenu()
{
    // Filter actions
    auto *filterClient = new QAction("Клиенту", this);
    auto *filterDate = new QAction("Дате", this);
    auto *filterPayment = new QAction("Статусу оплаты", this);

    filterClient->setData(static_cast<int>(FilterType::Client));
    filterDate->setData(static_cast<int>(FilterType::Date));
    filterPayment->setData(static_cast<int>(FilterType::Budget));

    QList<QAction*> filterActions = {
        filterClient,
        filterDate,
        filterPayment
    };

    m_filterGroup = new QActionGroup(this);
    m_filterGroup->setExclusive(true);

    for (auto action : filterActions) {
        action->setCheckable(true);
        m_filterGroup->addAction(action);
    }

    filterClient->setChecked(true);

    //Sort actions
    auto *sortClientAsc = new QAction("Клиенту (А–Я)", this);
    auto *sortClientDesc = new QAction("Клиенту (Я–А)", this);
    auto *sortDateAsc = new QAction("Дате (ближайшие)", this);
    auto *sortBudgetAsc = new QAction("Бюджету ↑", this);
    auto *sortBudgetDesc = new QAction("Бюджету ↓", this);

    sortClientAsc->setData(static_cast<int>(SortType::ClientAsc));
    sortClientDesc->setData(static_cast<int>(SortType::ClientDesc));
    sortDateAsc->setData(static_cast<int>(SortType::DateAsc));
    sortBudgetAsc->setData(static_cast<int>(SortType::BudgetAsc));
    sortBudgetDesc->setData(static_cast<int>(SortType::BudgetDesc));

    QList<QAction*> sortActions = {
        sortClientAsc,
        sortClientDesc,
        sortDateAsc,
        sortBudgetAsc,
        sortBudgetDesc
    };

    m_sortGroup = new QActionGroup(this);
    m_sortGroup->setExclusive(true);

    for (auto action : sortActions) {
        action->setCheckable(true);
        m_sortGroup->addAction(action);
    }

    sortDateAsc->setChecked(true);

    m_menu = new QMenu(this);
    m_menu->addSection("Искать по:");
    m_menu->addActions(filterActions);
    m_menu->addSection("Сортировать по:");
    m_menu->addActions(sortActions);

    m_filterButton->setMenu(m_menu);

    connect(m_filterGroup, &QActionGroup::triggered,
            this, &OrderSearchWidget::onFilterTriggered);

    connect(m_sortGroup, &QActionGroup::triggered,
            this, &OrderSearchWidget::onSortTriggered);
}

QString OrderSearchWidget::searchText() const
{
    return m_searchLineEdit->text();
}

OrderSearchWidget::FilterType OrderSearchWidget::filterType() const
{
    return m_currentFilter;
}

OrderSearchWidget::SortType OrderSearchWidget::sortType() const
{
    return m_currentSort;
}

void OrderSearchWidget::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text)
    emitSearchChanged();
}

void OrderSearchWidget::onFilterTriggered(QAction *action)
{
    m_currentFilter = static_cast<FilterType>(action->data().toInt());
    emitSearchChanged();
}

void OrderSearchWidget::onSortTriggered(QAction *action)
{
    m_currentSort = static_cast<SortType>(action->data().toInt());
    emitSearchChanged();
}

void OrderSearchWidget::emitSearchChanged()
{
    emit searchChanged(
        m_searchLineEdit->text(),
        m_currentFilter,
        m_currentSort
        );
}
