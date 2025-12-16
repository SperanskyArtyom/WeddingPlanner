#include "weddingorder.h"

#include <algorithm>

WeddingOrder::WeddingOrder(int id,
                           const QString &clientName,
                           const QDate &date,
                           double budget,
                           const QString &comment)
    : m_id(id)
    , m_clientName(clientName)
    , m_date(date)
    , m_budget(budget)
    , m_comment(comment)
{
    for (int i = 0; i < 4; i++)
    {
        Service::Type type = static_cast<Service::Type>(i);
        m_services[i].setType(type);
    }
}

int WeddingOrder::id() const
{
    return m_id;
}

const QString &WeddingOrder::clientName() const
{
    return m_clientName;
}

const QDate &WeddingOrder::date() const
{
    return m_date;
}

double WeddingOrder::budget() const
{
    return m_budget;
}

const QString &WeddingOrder::comment() const
{
    return m_comment;
}

const QString &WeddingOrder::servicePerformer(Service::Type type) const
{
    int index = static_cast<int>(type);
    return m_services[index].performerName();
}

Service WeddingOrder::service(Service::Type type) const
{
    int index = static_cast<int>(type);
    return m_services[index];
}

void WeddingOrder::setClientName(const QString &name)
{
    m_clientName = name;
}

void WeddingOrder::setDate(const QDate &date)
{
    m_date = date;
}

void WeddingOrder::setBudget(double budget)
{
    m_budget = budget;
}

void WeddingOrder::setComment(const QString &comment)
{
    m_comment = comment;
}

void WeddingOrder::setServicePerformer(Service::Type type, const QString &performerName)
{
    int index = static_cast<int>(type);
    m_services[index].setPerformerName(performerName);
}
