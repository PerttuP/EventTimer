/**
 * @file
 * @brief Implements the Event class defined in inc/event.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "event.hh"
#include <limits>
#include <QDateTime>

namespace EventTimerNS
{


const QString Event::TIME_FORMAT ("yyyy-MM-dd hh:mm:ss:zzz");
const unsigned Event::INFINITE_REPEAT (std::numeric_limits<unsigned>::max());
const unsigned Event::UNASSIGNED_ID (std::numeric_limits<unsigned>::max());


Event::Event() :
    name_(), timestamp_(), interval_(0), repeats_(0),
    type_(Event::STATIC), id_(UNASSIGNED_ID)
{
}


Event::Event(const QString& name,
             const QString& timestamp,
             Type type,
             unsigned interval,
             unsigned repeats) :

    name_(name), timestamp_(timestamp),
    interval_(interval), repeats_(repeats), type_(type), id_(UNASSIGNED_ID)
{
    Q_ASSERT(this->isValid());
}


Event::~Event()
{
}


Event Event::copy() const
{
    return Event(name(), timestamp(), type(), interval(), repeats());
}


QString Event::name() const
{
    return name_;
}


void Event::setName(const QString& name)
{
    name_ = name;
}


QString Event::timestamp() const
{
    return timestamp_;
}


void Event::setTimestamp(const QString& timestamp)
{
    timestamp_ = timestamp;
}


unsigned Event::interval() const
{
    return interval_;
}


void Event::setInterval(unsigned interval)
{
    interval_ = interval;
}


unsigned Event::repeats() const
{
    return repeats_;
}


void Event::setRepeats(unsigned repeats)
{
    repeats_ = repeats;
}


Event::Type Event::type() const
{
    return type_;
}


void Event::setType(Event::Type type)
{
    type_ = type;
}


unsigned Event::id() const
{
    return id_;
}


void Event::setId(unsigned id)
{
    Q_ASSERT (id_ == UNASSIGNED_ID);
    id_ = id;
}


bool Event::isValid() const
{
    return !name_.isEmpty() &&
            QDateTime::fromString(timestamp_, TIME_FORMAT).isValid() &&
            (repeats_ == 0 || interval_ != 0);
}


} // namespace EventTimerNS
