/**
 * @file
 * @brief Defines the DatabaseHandler class, that is responsible for
 *  initializing and communicating with the database.
 * @author Perttu Paarlahti 2016.
 */

#ifndef DATABASEHANDLER_HH
#define DATABASEHANDLER_HH

#include <QString>
#include <QSqlDatabase>
#include <vector>
#include "event.hh"

namespace EventTimerNS
{

/**
 * @brief The DatabaseHandler class takes care of making transactions in the database.
 */
class DatabaseHandler
{
public:

    /**
     * @brief Database setup parameters.
     */
    struct DbSetup
    {
        /**
         * @brief Database type string (refer to QtSql documentation for available type strings.
         */
        QString dbType;

        /**
         * @brief Name of the database.
         */
        QString dbName;

        /**
         * @brief Name of the used table in the database.
         */
        QString tableName;

        /**
         * @brief Database host name (remote databases only)
         */
        QString dbHostName;

        /**
         * @brief Database username. Leave empty if not required.
         */
        QString userName;

        /**
         * @brief Database password. Leave empty if not required.
         */
        QString password;
    };


    /**
     * @brief Constructor.
     * @param setup Database setup parameters.
     * @pre dbType, dbName and tableName are non-empty strings.
     * @post If initialization parameters are invalid or opening the database
     *  fails for other reasons, DatabaseHandler becomes invalid. Check isValid
     *  for validity and errorString for error message.
     */
    DatabaseHandler(const DbSetup& setup);

    /**
     * @brief Destructor.
     */
    ~DatabaseHandler();

    /**
     * @brief Copy-constructor is forbidden.
     */
    DatabaseHandler(const DatabaseHandler&) = delete;

    /**
     * @brief Assingment operator is forbidden.
     */
    DatabaseHandler& operator=(const DatabaseHandler&) = delete;

    /**
     * @brief Check if DatabaseHandler is in a valid state.
     * @return True, if current state is valid. If state is invalid, error
     *  message is available calling errorString().
     * @pre -
     */
    bool isValid() const;

    /**
     * @brief Get error message.
     * @return Message describing latest occured error.
     */
    QString errorString() const;

    /**
     * @brief Add event into the database.
     * @param e Event to be added.
     * @return Id assigned to the event.
     * @pre e != nullpre, e's id is unassigned.
     *  DatabaseHandler is in a valid state.
     * @post event is added or database is not changed.
     *  In case of error, returns -1 and updates the error string.
     */
    unsigned addEvent(Event* e);

    /**
     * @brief Remove event from the database.
     * @param eventId Event's unique id-number.
     * @return True, if event was removed successfully.
     * @pre DatabaseHandler is in a valid state.
     * @post Event is removed, or database is not modified. In case of error,
     * returns false and updates error string.
     */
    bool removeEvent(unsigned eventId);

    /**
     * @brief Get list of next events occuring after given time.
     * @param time Start time.
     * @param amount Up to how many events should the result list have.
     * @return Vector of up to  @p amount events occuring after @p time.
     * @pre @p time is in a valid format (Event::TIME_FORMAT) and represents a valid datetime.
     *   @p amount is not 0.
     * @post If query fails, returns empty vector and updates errorString().
     */
    std::vector<Event> nextEvents(QString time, unsigned amount);

    /**
     * @brief Remove all dynamic events from the database.
     * @return True, if all dynamic events were removed successfully.
     * @pre DatabaseHandler is in a valid state.
     * @post All dynamic events are removed or database is not modified.
     *  In case of error returns false and updates error string.
     */
    bool clearDynamic();

    /**
     * @brief Remove all events from database.
     * @return True, if all events were removed successfully.
     * @pre DatabaseHandler is an a valid state.
     * @post All events are removed or database is not modified.
     *  In case of error returns false and updates error string.
     */
    bool clearAll();

    /**
     * @brief Check for occured events.
     * @param time Inspected time.
     * @return Events occured before given time.
     * @pre time is in valid format (Event::TIME_FORMAT) and represents a valid datetime.
     */
    std::vector<Event> checkOccured(const QString& time);

    /**
     * @brief Update event name, time, type, interval and repeats.
     * @param eventID Id-number of the original event.
     * @param e Replacing event containig fields to be updated.
     * @return True, if update was successful.
     * @pre DatabaseHandler is in a valid state.
     * @post Updates the event or does not modify the database.
     *  In case of error returns false and updates the error string.
     */
    bool updateEvent(unsigned eventID, const Event& e);

    /**
     * @brief Get event matching the id number.
     * @param eventId Searched id number.
     * @return Event matching the id number. If no such event exists or query fails,
     *  returns event with id = -1. Extra information is available calling errorString().
     * @pre id > 0. DatabaseHandler is in a valid state.
     */
    Event getEvent(unsigned eventId);


private:

    QSqlDatabase db_;
    QString errorString_;
    bool errorFlag_;
    QString tableName_;

    static const QString CONNECTION_STRING_;
    static int connectionCount_;


    void openDB(const DbSetup& setup);
};

} // namespace EventTimerNS

#endif // DATABASEHANDLER_HH
