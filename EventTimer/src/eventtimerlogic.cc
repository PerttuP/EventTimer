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
    logger_(nullptr), refreshRate_(refreshRate), updateTimer_()
{
    Q_ASSERT(refreshRate >= 0);
    Q_ASSERT(dbHandler_ != nullptr);

    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(checkEvents()) );

    if (refreshRate_ != 0){
        updateTimer_.setInterval(refreshRate);
    }
}


EventTimerLogic::~EventTimerLogic()
{
}


unsigned EventTimerLogic::addEvent(Event* e)
{
    Q_ASSERT(e != nullptr);
    Q_ASSERT(e->isValid());
    Q_ASSERT(e->id() == Event::UNASSIGNED_ID);

    unsigned id = dbHandler_->addEvent(e);
    if (id == Event::UNASSIGNED_ID){
        this->logMessage("Could not add event: " + this->errorString());
    } else {
        this->logMessage("Event added. Id = " + QString::number(id));
    }

    if (refreshRate_ == 0 && updateTimer_.isActive()){
        this->setTimerToNextEvent();
    }
    return id;
}


bool EventTimerLogic::removeEvent(unsigned eventId)
{
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


Event EventTimerLogic::getEvent(unsigned eventId)
{
    Event e = dbHandler_->getEvent(eventId);
    if (e.id() == Event::UNASSIGNED_ID) {
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


std::vector<Event> EventTimerLogic::nextEvents(unsigned amount)
{
    Q_ASSERT(amount != 0);

    std::vector<Event> events = dbHandler_->nextEvents(QDateTime::currentDateTime().toString(Event::TIME_FORMAT), amount);

    if (events.size() == 0 && !dbHandler_->errorString().isEmpty()){
        logMessage("Could not get next events: " + dbHandler_->errorString());
    }

    return events;
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


void EventTimerLogic::start(CleanupPolicy policy)
{
    Q_ASSERT(eventHandler_ != nullptr);
    Q_ASSERT(this->isValid());
    Q_ASSERT(!updateTimer_.isActive());

    // Remove expired and dynamic events
    this->clearDynamic();
    std::vector<Event> events = dbHandler_->checkOccured(QDateTime::currentDateTime().toString(Event::TIME_FORMAT));
    for (Event e : events) {
        this->updateExpired(e);
        if (policy == NOTIFY){
            eventHandler_->notify(e);
        }
    }

    if (refreshRate_ == 0){
        this->setTimerToNextEvent();
    } else {
        updateTimer_.start();
    }
    logMessage("Timer started.");
}


void EventTimerLogic::stop()
{
    Q_ASSERT(updateTimer_.isActive());
    updateTimer_.stop();
}


void EventTimerLogic::checkEvents()
{
    // Get events from db.
    std::vector<Event> expired = dbHandler_->checkOccured(QDateTime::currentDateTime().toString(Event::TIME_FORMAT));
    if (expired.empty()){
        if (!dbHandler_->errorString().isEmpty()){
            this->logMessage("Could not check for events: " + this->errorString());
        }
        return;
    }

    // Update or remove events.
    for (Event e : expired) {
        this->updateExpired(e);
    }

    // Notify event handler.
    for (Event e : expired) {
        eventHandler_->notify(e);
    }

    if (refreshRate_ == 0){
        this->setTimerToNextEvent();
    }
}


void EventTimerLogic::logMessage(const QString& msg)
{
    if (logger_ != nullptr){
        logger_->logMsg(msg);
    }
}


bool EventTimerLogic::updateExpired(const Event& e)
{
    QDateTime current = QDateTime::currentDateTime();
    unsigned repeats_left = e.repeats();
    QDateTime nextTime = QDateTime::fromString(e.timestamp(), Event::TIME_FORMAT);

    // Find next occurence time after current time.
    while (nextTime < current) {
        if (repeats_left == 0) break;
        nextTime = nextTime.addMSecs(e.interval());
        if (repeats_left != Event::INFINITE_REPEAT){
            repeats_left--;
        }
    }

    if (nextTime < current){
        // Repeat times have run out.
        this->removeEvent(e.id());
        return true;
    }
    else {
        // Update timestamp and repeats.
        Event updated(e.name(), nextTime.toString(Event::TIME_FORMAT),
                      e.type(), e.interval(), repeats_left);
        dbHandler_->updateEvent(e.id(), updated);
    }
    return false;
}


void EventTimerLogic::setTimerToNextEvent()
{
    std::vector<Event> next = dbHandler_->nextEvents(QDateTime::currentDateTime().toString(Event::TIME_FORMAT), 1);
    if (next.empty()) return;

    QDateTime nextTime = QDateTime::fromString(next[0].timestamp(), Event::TIME_FORMAT);
    int diff = QDateTime::currentDateTime().msecsTo(nextTime);
    Q_ASSERT(diff >= 0);

    updateTimer_.start(diff);
}

} // namespace EventTimerNS
