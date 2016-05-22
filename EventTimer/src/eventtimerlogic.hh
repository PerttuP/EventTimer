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
#include <QTimer>
#include <QObject>

namespace EventTimerNS
{

/**
 * @brief The EventTimerLogic class implements the EventTimer interface.
 */
class EventTimerLogic : public QObject, public EventTimer
{
    Q_OBJECT

public:

    /**
     * @brief Constructor.
     * @param dbHandler DatabaseHandler.
     * @param refreshRate Event schedule refresh rate in milliseconds.
     * @pre refreshRate >= 0.
     */
    EventTimerLogic(std::unique_ptr<DatabaseHandler> dbHandler,
                    int refreshRate, QObject* parent = 0);

    /**
     * @brief Destructor.
     */
    virtual ~EventTimerLogic();

    // EventTimer interface
    virtual unsigned addEvent(Event* e);
    virtual bool removeEvent(unsigned eventId);
    virtual Event getEvent(unsigned eventId);
    virtual std::vector<Event> nextEvents(unsigned amount);
    virtual bool clearDynamic();
    virtual bool clearAll();
    virtual void setEventHandler(EventHandler* handler);
    virtual void setLogger(Logger* logger);
    virtual QString errorString() const;
    virtual bool isValid() const;
    virtual void start(CleanupPolicy policy);
    virtual void stop();


private slots:

    /**
     * @brief Check database for expired events and notify the EventHandler.
     */
    void checkEvents();


private:

    std::unique_ptr<DatabaseHandler> dbHandler_;
    EventHandler* eventHandler_;
    Logger* logger_;
    int refreshRate_;
    QTimer updateTimer_;

    void logMessage(const QString& msg);

    bool updateExpired(const Event& e);

    void setTimerToNextEvent();
};

} // namespace EventTimerNS

#endif // EVENTTIMERLOGIC_HH
