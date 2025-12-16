#include "weddingorder.h"

#include <algorithm>

WeddingOrder::WeddingOrder(int id,
                           const std::string &clientName,
                           const QDate &date,
                           double budget,
                           const std::string &comment)
    : m_id(id)
    , m_clientName(clientName)
    , m_date(date)
    , m_budget(budget)
    , m_comment(comment)
{
}

int WeddingOrder::id() const
{
    return m_id;
}

const std::string &WeddingOrder::clientName() const
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

const std::string &WeddingOrder::comment() const
{
    return m_comment;
}

const std::vector<Service> &WeddingOrder::services() const
{
    return m_services;
}

void WeddingOrder::setClientName(const std::string &name)
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

void WeddingOrder::setComment(const std::string &comment)
{
    m_comment = comment;
}

void WeddingOrder::addService(const Service &service)
{
    m_services.push_back(service);
}

void WeddingOrder::removeServiceById(int serviceId)
{
    m_services.erase(
        std::remove_if(m_services.begin(), m_services.end(),
                       [serviceId](const Service &s) {
                           return s.id() == serviceId;
                       }),
        m_services.end());
}

void WeddingOrder::clearServices()
{
    m_services.clear();
}
