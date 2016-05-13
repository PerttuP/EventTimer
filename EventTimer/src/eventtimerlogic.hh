/**
 * @file
 * @brief Defines the EventTimerLogic class, which implements the EventTimer interface.
 * @author Perttu Paarlahti 2016.
 */

#ifndef EVENTTIMERLOGIC_HH
#define EVENTTIMERLOGIC_HH

#include "eventtimer.hh"
#include "databasehandler.hh"
#include <memory>

namespace EventTimerNS
{

/**
 * @brief The EventTimerLogic class implements the EventTimer interface.
 */
class EventTimerLogic : public EventTimer
{
public:

    /**
     * @brief Constructor.
     * @param dbHandler DatabaseHandler.
     * @param refreshRate Event schedule refresh rate in milliseconds.
     * @pre refreshRate > 0.
     */
    EventTimerLogic(std::unique_ptr<DatabaseHandler> dbHandler,
                    int refreshRate);

    /**
     * @brief Destructor.
     */
    virtual ~EventTimerLogic();

    // EventTimer interface
    virtual int addEvent(Event* e);
    virtual bool removeEvent(int eventId);
    virtual bool clearDynamic();
    virtual bool clearAll();
    virtual void setEventHandler(EventHandler* handler);
    virtual void setLogger(Logger* logger);
    virtual QString errorString() const;
    virtual bool isValid() const;
    virtual void start();
    virtual void stop();
};

} // namespace EventTimerNS

#endif // EVENTTIMERLOGIC_HH
