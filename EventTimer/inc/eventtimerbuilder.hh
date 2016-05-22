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
        /**
         * @brief Database type. Refer to QSqlDriver documentation for avalable type strings.
         */
        QString dbType;

        /**
         * @brief Name of the database.
         */
        QString dbName;

        /**
         * @brief Name of used table in database.
         */
        QString tableName;

        /**
         * @brief Database host name. Leave empty if not required.
         */
        QString dbHostName;

        /**
         * @brief Database user name. Leave empty if not required.
         */
        QString userName;

        /**
         * @brief Database password. Leave empty if not required.
         */
        QString password;

        /**
         * @brief How often EventTimer will check for occured events (in milliseconds).
         * Value 0 has a special meaning: If refreshRate is 0, EventTimer
         * does not poll the database but calculates the next actual occurence
         * time and sets timer to it.
         */
        int refreshRateMsec;
    };

    /**
     * @brief Instantiate EventTimer component.
     * @param conf Configuration parameters.
     * @return New instance of EventTimer. Ownership is passed to the caller.
     * @pre RefreshRate >= 0. Instantiate only one EventTimer using same table at the same time.
     * @post New instance of EventTimer is created, but may not be in a valid state.
     *  Check validaty using EventTimer's isValid method. Discard invalid EventTimer.
     */
    static EventTimer* create(const Configuration& conf);
};

} // namespace EventTimerNS

#endif // EVENTTIMERBUILDER_HH
