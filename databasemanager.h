#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QList>
#include "weddingorder.h"
#include "service.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    enum class OrderFilter
    {
        Client,
        Date,
        Budget
    };
    Q_ENUM(OrderFilter)

    enum class OrderSort
    {
        ClientAsc,
        ClientDesc,
        DateAsc,
        BudgetAsc,
        BudgetDesc
    };
    Q_ENUM(OrderSort)

    bool openDatabase(const QString &path);
    void closeDatabase();

    QSqlDatabase& database();
    bool isOpen() const;

QList<WeddingOrder> getOrders(OrderFilter filter,
                              OrderSort sort,
                              const QString &filterText);

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
