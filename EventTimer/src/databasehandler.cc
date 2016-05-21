/**
 * @file
 * @brief Implements the DatabaseHandler class defined in src/databasehandler.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "databasehandler.hh"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

namespace EventTimerNS
{

const QString DatabaseHandler::CONNECTION_STRING_("EventTimerDbConnection");
int DatabaseHandler::connectionCount_(0);

DatabaseHandler::DatabaseHandler(const DbSetup& setup) :

    db_(), errorString_(), errorFlag_(false), tableName_(setup.tableName)
{
    Q_ASSERT(!setup.dbType.isEmpty());
    Q_ASSERT(!setup.dbName.isEmpty());
    Q_ASSERT(!setup.tableName.isEmpty());
    this->openDB(setup);
}


DatabaseHandler::~DatabaseHandler()
{
    db_.close();
}


bool DatabaseHandler::isValid() const
{
    return db_.isValid() && !errorFlag_;
}


QString DatabaseHandler::errorString() const
{
    return errorString_;
}


unsigned DatabaseHandler::addEvent(Event* e)
{
    Q_ASSERT(e != nullptr);
    Q_ASSERT(e->id() == Event::UNASSIGNED_ID);
    Q_ASSERT(this->isValid());

    QSqlQuery q("INSERT INTO " + tableName_ +
                " (name, timestamp, interval, repeats, static)"
                " VALUES('" + e->name() + "', '" + e->timestamp() + "', " +
                QString::number(e->interval()) + ", " +
                QString::number(e->repeats()) + ", " +
                (e->type() == Event::STATIC ? "1" : "0") +
                ")", db_);

    if (q.lastError().type() != QSqlError::NoError) {
        errorString_ = q.lastError().text();
        return -1;
    }

    // Find out the id of latest insertion (hackish).
    QSqlQuery q2("SELECT MAX(id) FROM " + tableName_, db_);
    q2.next();
    int latestId = q2.value(0).toInt();
    e->setId(latestId);
    return latestId;
}


bool DatabaseHandler::removeEvent(unsigned eventId)
{
    Q_ASSERT(this->isValid());

    QSqlQuery q("DELETE FROM " + tableName_ + " WHERE id = " + QString::number(eventId), db_);
    if (q.lastError().type() != QSqlError::NoError) {
        errorString_ = q.lastError().text();
        return false;
    }
    return true;
}


std::vector<Event> DatabaseHandler::nextEvents(QString time, unsigned amount)
{
    Q_ASSERT( QDateTime::fromString(time, Event::TIME_FORMAT).isValid() );
    Q_ASSERT( amount != 0 );

    // Execute query.
    QSqlQuery q("SELECT * FROM " + tableName_ + " WHERE timestamp > '" + time + "' ORDER BY timestamp", db_);
    if (q.lastError().type() != QSqlError::NoError){
        errorString_ = q.lastError().text();
        return std::vector<Event>();
    }

    // Gather list of up to 'amount' events from query results.
    std::vector<Event> events;
    unsigned i=0;
    while (i<amount && q.next()){
        Event e;
        e.setId( q.value("id").toUInt() );
        e.setName( q.value("name").toString() );
        e.setTimestamp( q.value("timestamp").toString() );
        e.setType( q.value("static").toInt() == 0 ? Event::DYNAMIC : Event::STATIC );
        e.setInterval( q.value("interval").toUInt() );
        e.setRepeats( q.value("repeats").toUInt() );
        Q_ASSERT(e.isValid());
        events.push_back(e);
        ++i;
    }

    errorString_.clear();
    return events;
}


bool DatabaseHandler::clearDynamic()
{
    Q_ASSERT (this->isValid());

    QSqlQuery q("DELETE FROM " + tableName_ + " WHERE static = 0", db_);
    if (q.lastError().type() != QSqlError::NoError) {
        errorString_ = q.lastError().text();
        return false;
    }
    return true;
}


bool DatabaseHandler::clearAll()
{
    Q_ASSERT (this->isValid());

    QSqlQuery q("DELETE FROM " + tableName_, db_);
    if (q.lastError().type() != QSqlError::NoError) {
        errorString_ = q.lastError().text();
        return false;
    }
    return true;
}


std::vector<Event> DatabaseHandler::checkOccured(const QDateTime& time)
{
    Q_ASSERT(time.isValid());
    Q_ASSERT(this->isValid());

    // Fetch event data.
    QSqlQuery q("SELECT * FROM " + tableName_ +
                " WHERE timestamp < '" +
                time.toString("yyyy-MM-dd hh:mm:ss:zzz") + "'", db_);

    if (q.lastError().type() != QSqlError::NoError) {
        errorString_ = q.lastError().text();
        return std::vector<Event>();
    }

    // Parse events.
    std::vector<Event> events;
    while (q.next()) {
        Event e(q.value("name").toString(),
                q.value("timestamp").toString(),
                q.value("static").toString() == "1" ? Event::STATIC : Event::DYNAMIC,
                q.value("interval").toUInt(),
                q.value("repeats").toUInt());
        e.setId(q.value("id").toInt());
        events.push_back(e);
    }
    return events;
}


bool DatabaseHandler::updateEvent(unsigned eventID, const Event& e)
{
    Q_ASSERT(this->isValid());

    QSqlQuery q("UPDATE "+ tableName_ +
                " SET name = "  + "'" + e.name() + "'," +
                " timestamp = " + "'" + e.timestamp() + "'," +
                " interval = "  + QString::number(e.interval()) + ","
                " repeats = "   + QString::number(e.repeats()) + ","
                " static = "    + (e.type() == Event::STATIC ? "1" : "0") +
                " WHERE id = "  + QString::number(eventID),
                db_);

    if (q.lastError().type() != QSqlError::NoError){
        errorString_ = q.lastError().text();
        return false;
    }
    return true;
}


Event DatabaseHandler::getEvent(unsigned eventId)
{
    Q_ASSERT(this->isValid());

    QSqlQuery q("SELECT id, name, timestamp, interval, repeats, static "
                "FROM " + tableName_ + " WHERE id = " + QString::number(eventId),
                db_);

    // Query failed.
    if (q.lastError().type() != QSqlError::NoError) {
        errorString_ = q.lastError().text();
        return Event("Query Failed", "2000-01-01 00:00:00:000", Event::DYNAMIC);
    }

    // No results.
    if (!q.next()){
        errorString_ = "";
        return Event("Not Found", "2000-01-01 00:00:00:000", Event::DYNAMIC);
    }

    // Create event.
    Event e(q.value("name").toString(),
            q.value("timestamp").toString(),
            (q.value("static").toString() == "1" ? Event::STATIC : Event::DYNAMIC),
            q.value("interval").toInt(), q.value("repeats").toInt());
    e.setId(q.value("id").toInt());
    return e;
}


void DatabaseHandler::openDB(const DbSetup& setup)
{
    db_ = QSqlDatabase::addDatabase(setup.dbType, CONNECTION_STRING_ + QString::number(connectionCount_));
    ++connectionCount_;
    db_.setDatabaseName(setup.dbName);

    if (!setup.dbHostName.isEmpty()) db_.setHostName(setup.dbHostName);
    if (!setup.userName.isEmpty())   db_.setUserName(setup.userName);
    if (!setup.password.isEmpty())   db_.setPassword(setup.password);

    if (db_.open()) {
        // Create table id not created.
        QSqlQuery q("CREATE TABLE IF NOT EXISTS " + tableName_ +
                    " (id INTEGER PRIMARY KEY,"
                    " name TEXT, timestamp TEXT, interval INTEGER,"
                    " repeats INTEGER, static INTEGER )",
                    db_);

        if (q.lastError().type() != QSqlError::NoError) {
            errorString_ = q.lastError().text();
            errorFlag_ = true;
        }
    }
    else {
        errorString_ = db_.lastError().text();
    }
}

} // namespace EventTimerNS
