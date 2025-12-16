#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QList>
#include "weddingorder.h"
#include "service.h"
#include "tasklist.h"

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
    WeddingOrder getOrder(int id);
    QList<QString> getAvailablePerformers(Service::Type type, const QDate &date);
    double getPaidAmount(int orderId) const;
    QList<Task> getTasksForOrder(int orderId);

    int addOrder(const WeddingOrder &order);
    bool saveService(int orderId, const Service &service);
    bool addPayment(int orderId, double amount);
    bool saveTask(int orderId, Task &task);

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
