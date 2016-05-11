/**
 * @file
 * @brief Defines the DatabaseHandler class, that is responsible for
 *  initializing and communicating with the database.
 * @author Perttu Paarlahti 2016.
 */

#ifndef DATABASEHANDLER_HH
#define DATABASEHANDLER_HH

#include <QString>
#include <QDateTime>
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
        QString dbType;
        QString dbName;
        QString tableName;
        QString dbHostName;
        QString userName;
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
    int addEvent(Event* e);

    /**
     * @brief Remove event from the database.
     * @param eventId Event's unique id-number.
     * @return True, if event was removed successfully.
     * @pre DatabaseHandler is in a valid state.
     * @post Event is removed, or database is not modified. In case of error,
     * returns false and updates error string.
     */
    bool removeEvent(int eventId);

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
     * @pre time is valid.
     */
    std::vector<Event> checkOccured(const QDateTime& time);

    /**
     * @brief Update event name, time, type, interval and repeats.
     * @param eventID Id-number of the original event.
     * @param e Replacing event containig fields to be updated.
     * @return True, if update was successful.
     * @pre DatabaseHandler is in a valid state.
     * @post Updates the event or does not modify the database.
     *  In case of error returns false and updates the error string.
     */
    bool updateEvent(int eventID, const Event& e);

    /**
     * @brief Get event matching the id number.
     * @param eventId Searched id number.
     * @return Event matching the id number. If no such event exists or query fails,
     *  returns event with id = -1. Extra information is available calling errorString().
     * @pre id > 0. DatabaseHandler is in a valid state.
     */
    Event getEvent(int eventId);


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
