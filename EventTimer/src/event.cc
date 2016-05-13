/**
 * @file
 * @brief Implements the Event class defined in inc/event.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "event.hh"

namespace EventTimerNS
{


const QString Event::TIME_FORMAT ("yyyy-MM-dd hh:mm:ss:zzz");


Event::Event() :
    name_(), timestamp_(), interval_(0), repeats_(0), type_(Event::STATIC), id_(-1)
{
}


Event::Event(const QString& name,
             const QString& timestamp,
             Type type,
             unsigned interval,
             unsigned repeats) :

    name_(name), timestamp_(timestamp),
    interval_(interval), repeats_(repeats), type_(type), id_(-1)
{
    Q_ASSERT(!name.isEmpty());
    Q_ASSERT(interval == 0 ? repeats == 0 : true);
}

Event::~Event()
{
}

QString Event::name() const
{
    return name_;
}

QString Event::timestamp() const
{
    return timestamp_;
}

unsigned Event::interval() const
{
    return interval_;
}

unsigned Event::repeats() const
{
    return repeats_;
}

Event::Type Event::type() const
{
    return type_;
}

int Event::id() const
{
    return id_;
}

void Event::setId(int id)
{
    Q_ASSERT (id_ == -1);
    Q_ASSERT (id >= 0);
    id_ = id;
}

} // namespace EventTimerNS
