/**
 * @file
 * @brief Defines the EventTimer interface - interface for using the EventTimer component.
 * @author Perttu Paarlahti 2016.
 */

#ifndef EVENTTIMER
#define EVENTTIMER

#include "event.hh"
#include "eventhandler.hh"
#include "logger.hh"

namespace EventTimerNS
{

/**
 * @brief Interface for the EventTimer component.
 *  This interface allows user to schedule events and cancel them.
 *  User will be notified about occuring events via user-provided
 *  EventHandler. User may receive log messages from the timer by
 *  providing Logger implementation.
 */
class EventTimer
{
public:

    /**
     * @brief Defines, what happens to expired static events when
     *  timer is started. This is an optional parameter for the start method.
     *  CLEAR-option clears static expired events without norifying the EventHandler.
     *  NOTIFY-option clears expired events and notifies the EventHandler once for each distinct event.
     */
    enum CleanupPolicy
    {
        CLEAR, NOTIFY
    };

    /**
     * @brief Mandatory virtual destructor.
     */
    virtual ~EventTimer() {}

    /**
     * @brief Time new event.
     * @param e Event to be timed.
     * @return Event id assigned to the event. If adding event fails, returns -1.
     * @pre Event != nullptr. Event id is unassigned.
     * @post Times the event and assigns it's id. If adding event fails,
     *  returns -1 and does not modify event. Error message is available calling errorString().
     */
    virtual int addEvent(Event* e) = 0;

    /**
     * @brief Cancel timed event.
     * @param eventId Id of event to be cancelled.
     * @return true, if event was cancelled successfully.
     * @pre eventId > 0.
     * @post Removes event or returns false and does not modify schedules.
     *  In case of failure, error message is available calling errorString().
     */
    virtual bool removeEvent(int eventId) = 0;

    /**
     * @brief Get event matching to the id.
     * @param eventId Event id.
     * @return Corresponding event with its current values
     *  (may have been updated since adding the event).
     *  If getting event fails, returns event with id == -1.
     * @pre eventID > 0.
     * @post If getting event fails, more info is available calling errorString().
     */
    virtual Event getEvent(int eventId) = 0;

    /**
     * @brief Remove all dynamic events from schedule.
     * @return true, if all dynamic events were removed.
     * @pre -
     * @post Removes all dynamic events or does not modify schedule.
     *  in case of error, error message is available calling errorString().
     */
    virtual bool clearDynamic() = 0;

    /**
     * @brief Clear the whole schedule.
     * @return True, if all events were removed successfully.
     * @pre -
     * @post Removes all events or does nothing. in case of error, error
     *  message is available calling errorString().
     */
    virtual bool clearAll() = 0;

    /**
     * @brief Assign handler for occured events.
     * @param handler EventHandler provided by component user.
     *  EventTimer does not take ownership over the handler.
     * @pre handler != null.
     * @post Handler will be notified when events occur.
     */
    virtual void setEventHandler(EventHandler* handler) = 0;

    /**
     * @brief Set log message handler.
     * @param logger Log messge handler provided by component user.
     *  EventTimer does not take ownership over the logger.
     * @pre -
     * @post Log messages will be passed to the logger.
     *  if this method is never called, or logger == nullptr, no log messages will be created.
     */
    virtual void setLogger(Logger* logger) = 0;

    /**
     * @brief Get error message.
     * @return Error message describing the latest occured error.
     */
    virtual QString errorString() const = 0;

    /**
     * @brief Check that EventTimer is in a valid state. Call this method after
     *  instantiation to verify successful initialization. If EventTimer is not in
     *  a valid state, error message is available calling errorString().
     * @return True, if EventTimer is in a valid state.
     * @pre -
     */
    virtual bool isValid() const = 0;

    /**
     * @brief Start or restart scheduling events.
     * @param policy Declares policy on expired static events.
     * @pre EventTimer is in a valid state. EventHandler has been set.
     * @post Event handler is norified about events occuring from now on.
     */
    virtual void start(CleanupPolicy policy = CLEAR) = 0;

    /**
     * @brief Stop scheduling events.
     * @pre -
     * @post EventHandler is no longer norified.
     */
    virtual void stop() = 0;
};

} // namespace EventTimerNS

#endif // EVENTTIMER

