#ifndef WEDDINGORDER_H
#define WEDDINGORDER_H

#include "service.h"

#include <QDate>
#include <string>
#include <vector>

class WeddingOrder
{
public:
    WeddingOrder() = default;

    WeddingOrder(int id,
                 const std::string &clientName,
                 const QDate &date,
                 double budget,
                 const std::string &comment = "");

    int id() const;
    const std::string &clientName() const;
    const QDate &date() const;
    double budget() const;
    const std::string &comment() const;
    const std::vector<Service> &services() const;

    void setClientName(const std::string &name);
    void setDate(const QDate &date);
    void setBudget(double budget);
    void setComment(const std::string &comment);

    void addService(const Service &service);
    void removeServiceById(int serviceId);
    void clearServices();

private:
    int m_id = -1;
    std::string m_clientName;
    QDate m_date;
    double m_budget = 0.0;
    std::string m_comment;
    std::vector<Service> m_services;
};

#endif // WEDDINGORDER_H
