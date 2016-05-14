/**
 * @file
 * @brief Implements the EventTimerLogic class defined in src/eventtimerlogic.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "eventtimerlogic.hh"
#include <QDateTime>


namespace EventTimerNS
{

EventTimerLogic::EventTimerLogic(std::unique_ptr<DatabaseHandler> dbHandler,
                                 int refreshRate, QObject* parent) :
    QObject(parent), EventTimer(),
    dbHandler_(std::move(dbHandler)), eventHandler_(nullptr),
    logger_(nullptr), updateTimer_()
{
    Q_ASSERT(dbHandler_ != nullptr);
    Q_ASSERT(refreshRate > 0)
    ;
    updateTimer_.setInterval(refreshRate);
    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(checkEvents()) );
}


EventTimerLogic::~EventTimerLogic()
{
}


int EventTimerLogic::addEvent(Event* e)
{
    Q_ASSERT(e != nullptr);
    Q_ASSERT(e->id() == -1);

    int id = dbHandler_->addEvent(e);
    if (id == -1){
        this->logMessage("Could not add event: " + this->errorString());
    } else {
        this->logMessage("Event added. Id = " + QString::number(id));
    }
    return id;
}


bool EventTimerLogic::removeEvent(int eventId)
{
    Q_ASSERT(eventId > 0);

    bool rv = dbHandler_->removeEvent(eventId);
    if (rv) {
        this->logMessage("Event removed (id = " + QString::number(eventId) + ").");
    } else {
        this->logMessage("Could not remove event (id = " +
                         QString::number(eventId) + "): " +
                         errorString() + ".");
    }
    return rv;
}


Event EventTimerLogic::getEvent(int eventId)
{
    Q_ASSERT(eventId > 0);

    Event e = dbHandler_->getEvent(eventId);
    if (e.id() == -1) {
        if (this->errorString().isEmpty()){
            logMessage("Could not get event (id=" +
                       QString::number(eventId) + "): " +
                       "No such event.");
        }
        else {
            logMessage("Could not get event (id=" +
                       QString::number(eventId) + "): " +
                       this->errorString() + ".");
        }
    }
    return e;
}


bool EventTimerLogic::clearDynamic()
{
    bool rv = dbHandler_->clearDynamic();
    if (rv) {
        logMessage("Dynamic events cleared successfully.");
    } else {
        logMessage("Dynamic events could not be cleared: " + errorString() + ".");
    }
    return rv;
}


bool EventTimerLogic::clearAll()
{
    bool rv = dbHandler_->clearAll();
    if (rv){
        this->logMessage("All events cleared successfully");
    } else {
        this->logMessage("Clearing events failed: " + this->errorString());
    }
    return rv;
}


void EventTimerLogic::setEventHandler(EventHandler* handler)
{
    Q_ASSERT (handler != nullptr);
    eventHandler_ = handler;
}


void EventTimerLogic::setLogger(Logger* logger)
{
    logger_ = logger;
}


QString EventTimerLogic::errorString() const
{
    return dbHandler_->errorString();
}


bool EventTimerLogic::isValid() const
{
    return dbHandler_->isValid();
}


void EventTimerLogic::start()
{
    Q_ASSERT(eventHandler_ != nullptr);
    Q_ASSERT(this->isValid());

    // Remove expired and dynamic events
    this->clearDynamic();
    std::vector<Event> events = dbHandler_->checkOccured(QDateTime::currentDateTime());
    for (Event e : events) {
        this->removeEvent(e.id());
    }

    updateTimer_.start();
}


void EventTimerLogic::stop()
{
    updateTimer_.stop();
}


void EventTimerLogic::checkEvents()
{
    // Get events from db.
    std::vector<Event> expired = dbHandler_->checkOccured(QDateTime::currentDateTime());
    if (expired.empty()){
        if (dbHandler_->errorString().isEmpty()){
            this->logMessage("Could not check for events: " + this->errorString());
        }
        return;
    }

    // Update or remove events.
    for (Event e : expired) {
        if (e.repeats() == 0){
            this->removeEvent(e.id());
        }
        else {
            // Update repreats and timestamp
            QDateTime newTime = QDateTime::fromString(e.timestamp(), Event::TIME_FORMAT).addMSecs(e.interval());
            Event updated(e.name(), newTime.toString(Event::TIME_FORMAT), e.type(), e.interval(), e.repeats()-1);
            if (!dbHandler_->updateEvent(e.id(), updated)){
                logMessage("Failed to update event (id=" +
                           QString::number(e.id()) + "): " +
                           errorString() );
            }
        }
    }

    // Notify event handler.
    for (Event e : expired) {
        eventHandler_->notify(e);
    }
}


void EventTimerLogic::logMessage(const QString& msg)
{
    if (logger_ != nullptr){
        logger_->logMsg(msg);
    }
}

} // namespace EventTimerNS
