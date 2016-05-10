/**
 * @file
 * @brief Implements the EventTimerLogic class defined in src/eventtimerlogic.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "eventtimerlogic.hh"

namespace EventTimerNS
{

EventTimerLogic::EventTimerLogic(DatabaseHandler dbHandler,
                                 int refreshRate)
{
    Q_ASSERT(dbHandler.isValid());
    Q_ASSERT(refreshRate > 0);
}


EventTimerLogic::~EventTimerLogic()
{
}


int EventTimerLogic::addEvent(Event* e)
{
    Q_ASSERT(e != nullptr);
    Q_ASSERT(e->id() == -1);
    return -1;
}


bool EventTimerLogic::removeEvent(int eventId)
{
    Q_ASSERT(eventId >= 0);
    return false;
}


bool EventTimerLogic::clearDynamic()
{
    return false;
}


bool EventTimerLogic::clearAll()
{
    return false;
}


void EventTimerLogic::setEventHandler(EventHandler* handler)
{
    Q_ASSERT (handler != nullptr);
}


void EventTimerLogic::setLogger(Logger* logger)
{
    Q_UNUSED(logger);
}


QString EventTimerLogic::errorString() const
{
    return QString("Not implemented yet!");
}


bool EventTimerLogic::isValid() const
{
    return false;
}


void EventTimerLogic::start()
{

}


void EventTimerLogic::stop()
{

}

} // namespace EventTimerNS
