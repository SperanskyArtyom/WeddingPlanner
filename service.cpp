#include "service.h"

Service::Service(int id,
                 Type type,
                 std::string &performerName)
    : m_id(id)
    , m_type(type)
    , m_performerName(performerName)
{
}

int Service::id() const
{
    return m_id;
}

Service::Type Service::type() const
{
    return m_type;
}

const std::string &Service::performerName() const
{
    return m_performerName;
}

void Service::setType(Type type)
{
    m_type = type;
}

void Service::setPerformerName(const std::string &name)
{
    m_performerName = name;
}
