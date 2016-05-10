/**
 * @file
 * @brief Implements the DatabaseHandler class defined in src/databasehandler.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "databasehandler.hh"

namespace EventTimerNS
{

DatabaseHandler::DatabaseHandler(const QString& dbType,
                                 const QString& dbName,
                                 const QString& tableName)
{
    Q_ASSERT(!dbType.isEmpty());
    Q_ASSERT(!dbName.isEmpty());
    Q_ASSERT(!tableName.isEmpty());
}

DatabaseHandler::~DatabaseHandler()
{
}


bool DatabaseHandler::isValid() const
{
    return false;
}

QString DatabaseHandler::errorString() const
{
    return QString();
}

int DatabaseHandler::addEvent(Event* e)
{
    Q_ASSERT(e != nullptr);
    Q_ASSERT(e->id == -1);
    return -1;
}

bool DatabaseHandler::removeEvent(int eventId)
{
    Q_UNUSED(eventId);
    return false;
}

bool DatabaseHandler::clearDynamic()
{
    return false;
}

bool DatabaseHandler::clearAll()
{
    return false;
}

std::vector<Event> DatabaseHandler::checkOccured(const QDateTime& time)
{
    Q_ASSERT(time.isValid());
    return std::vector<Event>();
}

bool DatabaseHandler::updateEvent(int eventID, const Event* e)
{
    Q_ASSERT(e != nullptr);
    Q_UNUSED(eventID);
    return false;
}

} // namespace EventTimerNS
