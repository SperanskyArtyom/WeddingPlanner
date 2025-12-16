#ifndef SERVICE_H
#define SERVICE_H

#include <string>

class Service
{
public:
    enum class Type
    {
        None,
        Photo,
        Host,
        Decor,
        Transport
    };

    Service() = default;

    Service(int id,
            Type type,
            std::string &performerName);

    int id() const;
    Type type() const;
    const std::string &performerName() const;

    void setType(Type type);
    void setPerformerName(const std::string &name);

private:
    int m_id = -1;
    Type m_type = Type::None;
    std::string m_performerName;
};

#endif // SERVICE_H
