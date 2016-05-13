/**
 * @file
 * @brief Implements the EventTimerBuilder class defined in inc/EventTimerBuilder.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "eventtimerbuilder.hh"
#include "databasehandler.hh"
#include "eventtimerlogic.hh"
#include <memory>

namespace EventTimerNS
{

EventTimer*EventTimerBuilder::create(const EventTimerBuilder::Configuration& conf)
{
    DatabaseHandler::DbSetup setup;
    setup.dbType = conf.dbType;
    setup.dbName = conf.dbName;
    setup.tableName = conf.tableName;
    setup.dbHostName = conf.dbHostName;
    setup.userName = conf.userName;
    setup.password = conf.password;

    std::unique_ptr<DatabaseHandler> dbHandler(new DatabaseHandler(setup));
    return new EventTimerLogic(std::move(dbHandler), conf.refreshRateMsec);
}

} // namespace EventTimerNS
