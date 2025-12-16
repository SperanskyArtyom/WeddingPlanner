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
            QDate::fromString(query.value(2).toString(), "dd.MM.yy"),
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

    QDate date = QDate::fromString(query.value("date").toString(), "dd.MM.yy");
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

QList<QString> DatabaseManager::getAvailablePerformers(Service::Type type, const QDate &date)
{
    QList<QString> result;

    if (!isOpen())
        return result;

    QSqlQuery query(m_db);

    query.prepare(R"(
        SELECT p.name
        FROM performers p
        WHERE p.type = :type
          AND p.name NOT IN (
              SELECT s.performer_name
              FROM services s
              JOIN orders o ON o.id = s.order_id
              WHERE s.type = :type
                AND o.date = :date
          )
        ORDER BY p.name
    )");

    query.bindValue(":type", static_cast<int>(type));
    query.bindValue(":date", date.toString("dd.MM.yy"));

    if (!query.exec())
    {
        qDebug() << "availablePerformers error:"
                 << query.lastError().text();
        return result;
    }

    while (query.next())
    {
        result.append(query.value(0).toString());
    }

    return result;
}

int DatabaseManager::addOrder(const WeddingOrder &order)
{
    if (!isOpen()) return -1;

    QSqlQuery query(m_db);

    query.prepare("INSERT INTO orders (date, budget, comment) "
                  "VALUES (:date, :budget, :comment)");
    query.bindValue(":date", order.date().toString("dd.MM.yy"));
    query.bindValue(":budget", order.budget());
    query.bindValue(":comment", order.comment());

    if (!query.exec()) {
        qDebug() << "Failed to insert order:" << query.lastError().text();
        return -1;
    }

    int orderId = static_cast<int>(query.lastInsertId().toInt());

    query.prepare("INSERT INTO clients (order_id, name) VALUES (:order_id, :name)");
    query.bindValue(":order_id", orderId);
    query.bindValue(":name", order.clientName());
    if (!query.exec()) {
        qDebug() << "Failed to insert client:" << query.lastError().text();
        return -1;
    }

    return orderId;
}

bool DatabaseManager::saveService(int orderId, const Service &service)
{
    if (!isOpen())
        return false;

    QSqlQuery query(m_db);

    if (service.id() == -1)
    {
        // INSERT
        query.prepare(R"(
            INSERT INTO services (order_id, type, performer_name)
            VALUES (:order_id, :type, :performer_name)
        )");

        query.bindValue(":order_id", orderId);
        query.bindValue(":type", static_cast<int>(service.type()));
        query.bindValue(":performer_name", service.performerName());

        if (!query.exec())
        {
            qDebug() << "Insert service error:" << query.lastError().text();
            return false;
        }

        // при необходимости можно получить новый id:
        // int newId = query.lastInsertId().toInt();
        // service.setId(newId);  ← только если service НЕ const
    }
    else
    {
        // UPDATE
        query.prepare(R"(
            UPDATE services
            SET performer_name = :performer_name
            WHERE id = :id
        )");

        query.bindValue(":performer_name", service.performerName());
        query.bindValue(":id", service.id());

        if (!query.exec())
        {
            qDebug() << "Update service error:" << query.lastError().text();
            return false;
        }
    }

    return true;
}

double DatabaseManager::getPaidAmount(int orderId) const
{
    if (!isOpen()) return 0.0;

    QSqlQuery query(m_db);
    query.prepare("SELECT SUM(amount) FROM payments WHERE order_id = :id");
    query.bindValue(":id", orderId);

    if (!query.exec()) {
        qDebug() << "getPaidAmount error:" << query.lastError().text();
        return 0.0;
    }

    if (query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

bool DatabaseManager::addPayment(int orderId, double amount)
{
    if (!isOpen()) return false;

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO payments (order_id, amount, created_at) "
                  "VALUES (:order_id, :amount, :created_at)");
    query.bindValue(":order_id", orderId);
    query.bindValue(":amount", amount);
    query.bindValue(":created_at", QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss"));

    if (!query.exec()) {
        qDebug() << "addPayment error:" << query.lastError().text();
        return false;
    }

    return true;
}

QList<Task> DatabaseManager::getTasksForOrder(int orderId)
{
    QList<Task> tasks;
    if(!isOpen()) return tasks;

    QSqlQuery query(m_db);
    query.prepare("SELECT id, section, title, is_done FROM tasks WHERE order_id = :orderId");
    query.bindValue(":orderId", orderId);

    if(!query.exec()) {
        qDebug() << "getTasksForOrder error:" << query.lastError().text();
        return tasks;
    }

    while(query.next()) {
        Task t;
        t.id = query.value("id").toInt();
        t.section = query.value("section").toString();
        t.label = query.value("title").toString();
        t.isDone = query.value("is_done").toInt() != 0;
        tasks.append(t);
    }

    return tasks;
}

bool DatabaseManager::saveTask(int orderId, Task &task)
{
    if(!isOpen()) return false;

    QSqlQuery query(m_db);

    if(task.id == -1) {
        query.prepare("INSERT INTO tasks (order_id, section, title, is_done) "
                      "VALUES (:order_id, :section, :title, :is_done)");
        query.bindValue(":order_id", orderId);
        query.bindValue(":section", task.section);
        query.bindValue(":title", task.label);
        query.bindValue(":is_done", task.isDone ? 1 : 0);

        if(!query.exec()) {
            qDebug() << "saveTask insert error:" << query.lastError().text();
            return false;
        }

        task.id = query.lastInsertId().toInt();
    } else {
        query.prepare("UPDATE tasks SET is_done = :is_done WHERE id = :id");
        query.bindValue(":is_done", task.isDone ? 1 : 0);
        query.bindValue(":id", task.id);

        if(!query.exec()) {
            qDebug() << "saveTask update error:" << query.lastError().text();
            return false;
        }
    }

    return true;
}
