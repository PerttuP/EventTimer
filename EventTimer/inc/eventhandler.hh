/**
 * @file
 * @brief Defines the EventHandler interface. Component user provides
 *  implementation for this interface.
 * @author Perttu Paarlahti 2016.
 */

#ifndef EVENTHANDLER_HH
#define EVENTHANDLER_HH

#include <QString>

namespace EventTimerNS
{

/**
 * @brief Iterface for handling occured events. Component user provides implementation.
 */
class EventHandler
{
public:

    /**
     * @brief Mandatory virtual constructor.
     */
    virtual ~EventHandler() {}

    /**
     * @brief Notify handler about occured event.
     * @param eventID Event's id-number.
     * @param eventName Event's name.
     * @pre eventID and eventName represent an existing event, that has occured.
     * @post Handler implementation takes care of handling the event.
     *  Event handling is expected to be re-entrant.
     */
    virtual void notify(int eventID, const QString& eventName) = 0;
};


} // namespace EventTimerNS

#endif // EVENTHANDLER_HH

