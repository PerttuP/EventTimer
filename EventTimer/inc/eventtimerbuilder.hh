/**
 * @file
 * @brief Defines the EventTimerBuilder class - Builder class for the
 *  EventTimer component.
 * @author Perttu Paarlahti 2016.
 */

#ifndef EVENTTIMERBUILDER_HH
#define EVENTTIMERBUILDER_HH

#include "eventtimer.hh"

namespace EventTimerNS
{

/**
 * @brief Builder class for EventTimer component.
 *  Use this class to instantiate EventTimers.
 */
class EventTimerBuilder
{
public:

    /**
     * @brief EventTimer configuration parameters.
     */
    struct Configuration
    {
        QString dbType;      // Database type. Refer to QSqlDriver documentation for avalable type strings.
        QString dbName;      // Name of the database.
        QString tableName;   // Name for used table in database.
        int refreshRateMsec; // How often EventTimer will check for occured events.
    };

    /**
     * @brief Instantiate EventTimer component.
     * @param conf Configuration parameters.
     * @return New instance of EventTimer. Ownership is passed to the caller.
     * @pre conf.refreshRateMsec > 0. Instantiate only one EventTimer using
     *  same table at the same time.
     * @post New instance of EventTimer is created, but may not be in a valid state.
     *  Check validaty using EventTimer's isValid method. Discard invalid EventTimer.
     */
    static EventTimer* create(const Configuration& conf);
};

} // namespace EventTimerNS

#endif // EVENTTIMERBUILDER_HH