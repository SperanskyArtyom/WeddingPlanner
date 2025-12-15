#ifndef ORDERSEARCHWIDGET_H
#define ORDERSEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMenu>
#include <QActionGroup>

class OrderSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderSearchWidget(QWidget *parent = nullptr);

    enum class FilterType {
        Client,
        Date,
        PaymentStatus
    };

    enum class SortType {
        ClientAsc,
        ClientDesc,
        DateAsc,
        BudgetAsc,
        BudgetDesc
    };

    QString searchText() const;
    FilterType filterType() const;
    SortType sortType() const;

signals:
    void searchChanged(const QString &text,
                       FilterType filter,
                       SortType sort);

private slots:
    void onSearchTextChanged(const QString &text);
    void onFilterTriggered(QAction *action);
    void onSortTriggered(QAction *action);

private:
    void initUi();
    void initFilterMenu();
    void emitSearchChanged();

    QLineEdit   *m_searchLineEdit;
    QPushButton *m_filterButton;

    QMenu *m_menu;
    QActionGroup *m_filterGroup;
    QActionGroup *m_sortGroup;

    FilterType m_currentFilter = FilterType::Client;
    SortType   m_currentSort   = SortType::DateAsc;
};

#endif // ORDERSEARCHWIDGET_H
