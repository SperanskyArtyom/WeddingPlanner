#include "databasemanager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QRegularExpression>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent}
{}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase(const QString &path)
{
    if (QSqlDatabase::contains("main_connection"))
        m_db = QSqlDatabase::database("main_connection");
    else
        m_db = QSqlDatabase::addDatabase("QSQLITE", "main_connection");

    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Cannot open database:" << m_db.lastError().text();
        return false;
    }

    return true;
}

void DatabaseManager::closeDatabase()
{
    if (isOpen())
        m_db.close();
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}

QList<WeddingOrder> DatabaseManager::getOrders(OrderFilter filter,
                                               OrderSort sort,
                                               const QString &filterText)
{
    QList<WeddingOrder> result;

    if (!isOpen())
        return result;

    QString trimmed = filterText.trimmed();
    bool useText = !trimmed.isEmpty();

    bool budgetFilterValid = false;
    QChar budgetOp;
    double budgetValue = 0.0;

    if (filter == OrderFilter::Budget && useText)
    {
        QRegularExpression re(R"(^([<>=])\s*(\d+(\.\d+)?)$)");
        QRegularExpressionMatch match = re.match(trimmed);

        if (match.hasMatch())
        {
            budgetOp = match.captured(1)[0];
            budgetValue = match.captured(2).toDouble();
            budgetFilterValid = true;
        }
    }

    QString queryStr =
        "SELECT o.id, c.name, o.date, o.budget, o.comment "
        "FROM orders o "
        "JOIN clients c ON c.order_id = o.id ";

    // --- WHERE ---
    switch (filter)
    {
    case OrderFilter::Client:
        if (useText)
            queryStr += "WHERE c.name LIKE :filter ";
        break;

    case OrderFilter::Date:
        if (useText)
            queryStr += "WHERE o.date LIKE :filter ";
        break;

    case OrderFilter::Budget:
        if (budgetFilterValid)
            queryStr += QString("WHERE o.budget %1 :budget ").arg(budgetOp);
        break;
    }

    // --- ORDER BY ---
    switch (sort)
    {
    case OrderSort::ClientAsc:
        queryStr += "ORDER BY c.name ASC ";
        break;

    case OrderSort::ClientDesc:
        queryStr += "ORDER BY c.name DESC ";
        break;

    case OrderSort::DateAsc:
        queryStr += "ORDER BY o.date ASC ";
        break;

    case OrderSort::BudgetAsc:
        queryStr += "ORDER BY o.budget ASC ";
        break;

    case OrderSort::BudgetDesc:
        queryStr += "ORDER BY o.budget DESC ";
        break;
    }

    QSqlQuery query(m_db);
    query.prepare(queryStr);

    if ((filter == OrderFilter::Client || filter == OrderFilter::Date) && useText)
    {
        query.bindValue(":filter", "%" + trimmed + "%");
    } else if (filter == OrderFilter::Budget && budgetFilterValid)
    {
        query.bindValue(":budget", budgetValue);
    }

    if (!query.exec())
    {
        qDebug() << "getOrders error:" << query.lastError().text();
        return result;
    }

    while (query.next())
    {
        WeddingOrder order(
            query.value(0).toInt(),
            query.value(1).toString(),
            QDate::fromString(query.value(2).toString(), Qt::ISODate),
            query.value(3).toDouble(),
            query.value(4).toString()
            );

        result.append(order);
    }

    return result;
}

WeddingOrder DatabaseManager::getOrder(int id)
{
    WeddingOrder order;

    if (!isOpen())
        return order;

    QSqlQuery query(m_db);

    query.prepare("SELECT date, budget, comment FROM orders WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        qDebug() << "Order not found or query failed:" << query.lastError().text();
        return order;
    }

    QDate date = QDate::fromString(query.value("date").toString(), Qt::ISODate);
    double budget = query.value("budget").toDouble();
    QString comment = query.value("comment").toString();

    QSqlQuery clientQuery(m_db);
    clientQuery.prepare("SELECT name FROM clients WHERE order_id = :id");
    clientQuery.bindValue(":id", id);
    QString clientName;
    if (clientQuery.exec() && clientQuery.next())
        clientName = clientQuery.value("name").toString();

    order = WeddingOrder(id, clientName, date, budget, comment);

    QSqlQuery serviceQuery(m_db);
    serviceQuery.prepare("SELECT type, performer_name FROM services WHERE order_id = :id");
    serviceQuery.bindValue(":id", id);

    if (serviceQuery.exec()) {
        while (serviceQuery.next()) {
            int typeInt = serviceQuery.value("type").toInt();
            QString performer = serviceQuery.value("performer_name").toString();
            Service::Type type = static_cast<Service::Type>(typeInt);
            order.setServicePerformer(type, performer);
        }
    } else {
        qDebug() << "Service query failed:" << serviceQuery.lastError().text();
    }

    return order;
}
