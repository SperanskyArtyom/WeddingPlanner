#ifndef WEDDINGORDER_H
#define WEDDINGORDER_H

#include "service.h"

#include <QDate>
#include <QString>

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
    const QString &servicePerformer(Service::Type type) const;

    void setClientName(const QString &name);
    void setDate(const QDate &date);
    void setBudget(double budget);
    void setComment(const QString &comment);
    void setServicePerformer(Service::Type type, const QString &performerName);

private:
    int m_id = -1;
    QString m_clientName;
    QDate m_date;
    double m_budget = 0.0;
    QString m_comment;
    Service m_services[4];
};

#endif // WEDDINGORDER_H
