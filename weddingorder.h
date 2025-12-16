#ifndef WEDDINGORDER_H
#define WEDDINGORDER_H

#include "service.h"

#include <QDate>
#include <QString>
#include <vector>

class WeddingOrder
{
public:
    WeddingOrder() = default;

    WeddingOrder(int id,
                 const QString &clientName,
                 const QDate &date,
                 double budget,
                 const QString &comment = QString());

    int id() const;
    const QString &clientName() const;
    const QDate &date() const;
    double budget() const;
    const QString &comment() const;
    const std::vector<Service> &services() const;

    void setClientName(const QString &name);
    void setDate(const QDate &date);
    void setBudget(double budget);
    void setComment(const QString &comment);

    void addService(const Service &service);
    void removeServiceById(int serviceId);
    void clearServices();

private:
    int m_id = -1;
    QString m_clientName;
    QDate m_date;
    double m_budget = 0.0;
    QString m_comment;
    std::vector<Service> m_services;
};

#endif // WEDDINGORDER_H
