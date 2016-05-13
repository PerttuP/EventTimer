/**
 * @file
 * @brief Benchmarking tests for the DatabaseHandler class.
 * @author Perttu Paarlahti 2016.
 */

#include <QString>
#include <QtTest>
#include <memory>
#include "databasehandler.hh"

/**
 * @brief The DatabaseHandlerBenchmark clas
 *  implements benchmarking for the DatabaseHandler class.
 */
class DatabaseHandlerBenchmark : public QObject
{
    Q_OBJECT

public:
    DatabaseHandlerBenchmark();

    ~DatabaseHandlerBenchmark();

private Q_SLOTS:

    /**
     * @brief Benchmark DatabaseHandler constructor.
     */
    void constructorBenchmark();
    void constructorBenchmark_data();

    /**
     * @brief Benchmark consequtive additions in an empty database.
     */
    void addEventBenchmark();
    void addEventBenchmark_data();

    /**
     * @brief Benchmark updating single event in the database.
     */
    void updateSingleBenchmark();
    void updateSingleBenchmark_data();

    /**
     * @brief Benchmark getting single event in the database.
     */
    void getEventSingle();
    void getEventSingle_data();

    /**
     * @brief Benchmark adding, updating and removing single element.
     */
    void addUpdateRemoveSingleEvent();
    void addUpdateRemoveSingleEvent_data();


private:

    std::shared_ptr<EventTimerNS::DatabaseHandler>
    initDB(QString dbType, QString dbName, QString tableName, QString dbHost, QString userName, QString password);
};


DatabaseHandlerBenchmark::DatabaseHandlerBenchmark()
{   
    qDebug() <<"Ctor";
}

DatabaseHandlerBenchmark::~DatabaseHandlerBenchmark()
{
    qDebug() << "Dtor";
}


void DatabaseHandlerBenchmark::constructorBenchmark()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    EventTimerNS::DatabaseHandler::DbSetup setup;
    setup.dbType = dbType;
    setup.dbName = dbName;
    setup.tableName = tableName;
    setup.dbHostName = dbHost;
    setup.userName = userName;
    setup.password = password;

    EventTimerNS::DatabaseHandler* h = nullptr;
    QBENCHMARK {
        h = new EventTimerNS::DatabaseHandler(setup);
    }
    delete h;
}


void DatabaseHandlerBenchmark::constructorBenchmark_data()
{
    QTest::addColumn<QString>("dbType");
    QTest::addColumn<QString>("dbName");
    QTest::addColumn<QString>("tableName");
    QTest::addColumn<QString>("dbHost");
    QTest::addColumn<QString>("userName");
    QTest::addColumn<QString>("password");

    QTest::newRow("Local SQLite no authentication")
            << "QSQLITE" << "SQLiteTestDB" << "events" << QString() << QString() << QString();
}


void DatabaseHandlerBenchmark::addEventBenchmark()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    QBENCHMARK {
        Event e("eventName", "2000-01-01 00:00:00:000", Event::STATIC, 1000, 123);
        handler->addEvent(&e);
    }

    handler->clearAll();
}


void DatabaseHandlerBenchmark::addEventBenchmark_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::updateSingleBenchmark()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    Event original("original", "2000-01-01 00:00:00:000", Event::DYNAMIC, 0, 0);
    Event updated("updated", "2016-05-13 17:11:00:000", Event::DYNAMIC, 10, 10);
    handler->addEvent(&original);

    QBENCHMARK {
        handler->updateEvent(original.id(), updated);
    }

    handler->clearAll();
}


void DatabaseHandlerBenchmark::updateSingleBenchmark_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::getEventSingle()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    Event original("original", "2000-01-01 00:00:00:000", Event::DYNAMIC, 0, 0);
    handler->addEvent(&original);
    Event tmp;

    QBENCHMARK {
        tmp = handler->getEvent(original.id());
    }

    handler->clearAll();
}


void DatabaseHandlerBenchmark::getEventSingle_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::addUpdateRemoveSingleEvent()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    QBENCHMARK {
        Event original("original", "2000-01-01 00:00:00:000", Event::DYNAMIC, 0, 0);
        Event updated("updated", "2016-05-13 17:11:00:000", Event::DYNAMIC, 10, 10);
        handler->addEvent(&original);
        handler->updateEvent(original.id(), updated);
        handler->removeEvent(original.id());
    }

    handler->clearAll();
}


void DatabaseHandlerBenchmark::addUpdateRemoveSingleEvent_data()
{
    constructorBenchmark_data();
}


std::shared_ptr<EventTimerNS::DatabaseHandler>
DatabaseHandlerBenchmark::initDB(QString dbType, QString dbName, QString tableName, QString dbHost, QString userName, QString password)
{
    EventTimerNS::DatabaseHandler::DbSetup setup;
    setup.dbType = dbType;
    setup.dbName = dbName;
    setup.tableName = tableName;
    setup.dbHostName = dbHost;
    setup.userName = userName;
    setup.password = password;

    std::shared_ptr<EventTimerNS::DatabaseHandler> h(new EventTimerNS::DatabaseHandler(setup));
    h->clearAll();
    return h;
}


QTEST_APPLESS_MAIN(DatabaseHandlerBenchmark)

#include "tst_databasehandlerbenchmark.moc"
