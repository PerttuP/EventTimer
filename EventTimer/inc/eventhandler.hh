/**
 * @file
 * @brief Defines the EventHandler interface. Component user provides
 *  implementation for this interface.
 * @author Perttu Paarlahti 2016.
 */

#ifndef EVENTHANDLER_HH
#define EVENTHANDLER_HH

#include <QString>
#include "event.hh"

namespace EventTimerNS
{

/**
 * @brief Iterface for handling occured events. Component user provides implementation.
 */
class EventHandler
{
public:

    /**
     * @brief Mandatory virtual destructor.
     */
    virtual ~EventHandler() {}

    /**
     * @brief Notify handler about occured event.
     * @param event Occured event. Event is in its pre-occurence state.
     * @pre Event is valid and represents an actual event that has occured.
     * @post Handler implementation takes care of handling the event.
     */
    virtual void notify(const Event& event) = 0;
};


} // namespace EventTimerNS

#endif // EVENTHANDLER_HH

