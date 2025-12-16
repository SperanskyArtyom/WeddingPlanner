#ifndef SERVICE_H
#define SERVICE_H

#include <QString>

class Service
{
public:
    enum class Type
    {
        Photo,
        Host,
        Decor,
        Transport
    };

    Service() = default;

    Service(int id,
            Type type,
            const QString &performerName);

    int id() const;
    Type type() const;
    const QString &performerName() const;

    void setType(Type type);
    void setPerformerName(const QString &name);

private:
    int m_id = -1;
    Type m_type;
    QString m_performerName;
};

#endif // SERVICE_H
