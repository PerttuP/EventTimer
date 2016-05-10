/**
 * @file
 * @brief Defines interface for handling log messages produced by EventTimer.
 *  Component user provides implementation for this interface.
 * @author Perttu Paarlahti 2016.
 */

#ifndef LOGGER_HH
#define LOGGER_HH

#include <QString>

namespace EventTimerNS
{

/**
 * @brief Interface for handling log messages. Component user provides implementation.
 */
class Logger
{
public:

    /**
     * @brief Mandatory virtual destructor.
     */
    virtual ~Logger() {}

    /**
     * @brief Write log.
     * @param msg Logged message.
     * @pre -
     * @post Logger implementation is expected to be re-entrant.
     */
    virtual void logMsg(const QString& msg) = 0;
};

} // namespace EventTimerNS

#endif // LOGGER_HH
