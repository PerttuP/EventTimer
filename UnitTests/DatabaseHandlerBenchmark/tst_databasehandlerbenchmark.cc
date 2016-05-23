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
     * @brief Test getting next events in relatively small db (get 3 out of 20).
     */
    void nextEventsBenchmark();
    void nextEventsBenchmark_data();

    /**
     * @brief Benchmark adding, updating and removing single element.
     */
    void addUpdateRemoveSingleEvent();
    void addUpdateRemoveSingleEvent_data();


    // Big data tests (1000 events):
    // ------------------------------------------------------------------------

    /**
     * @brief Benchmark adding 1000 events into empty database.
     */
    void addThousandEvents();
    void addThousandEvents_data();

    /**
     * @brief Benchmark updating 1000 different events in the database.
     */
    void updateThousandEvents();
    void updateThousandEvents_data();

    /**
     * @brief Benchmark getting 1000 different events from the database.
     */
    void getThousandEvents();
    void getThousandEvents_data();

    /**
     * @brief Benchmark getting 500 expired events from the database.
     */
    void get500ExpiredEvents();
    void get500ExpiredEvents_data();

    /**
     * @brief Benchmark getting next event from db of 1000 events.
     */
    void oneNextEventsFromThousandEvents();
    void oneNextEventsFromThousandEvents_data();

    /**
     * @brief Benchmark getting 1000 next events from db of 1001 events.
     */
    void thousandNextEvents();
    void thousandNextEvents_data();

    /**
     * @brief Benchmark clearing 500 dynamic events.
     */
    void clear500DynamicEvents();
    void clear500DynamicEvents_data();

    /**
     * @brief Benchmark clearing remaining 500 events.
     */
    void clear500Events();
    void clear500Events_data();

    /**
     * @brief Benchmark removing 1000 events from the database one by one.
     */
    void removeThousandEvents();
    void removeThousandEvents_data();


private:

    std::shared_ptr<EventTimerNS::DatabaseHandler>
    initDB(QString dbType, QString dbName, QString tableName, QString dbHost, QString userName, QString password);

    // Container for storing Events in big data tests.
    std::vector<EventTimerNS::Event> events_;
    QDateTime currentTime_;
};


DatabaseHandlerBenchmark::DatabaseHandlerBenchmark()
{   
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
    handler->clearAll();

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
    handler->clearAll();

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
    handler->clearAll();

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


void DatabaseHandlerBenchmark::nextEventsBenchmark()
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
    handler->clearAll();

    // Add 17 past events
    for (unsigned i=1; i<=17; ++i){
        Event e("past"+QString::number(i),
                QDateTime::currentDateTime().addDays(-i).toString(Event::TIME_FORMAT),
                Event::STATIC, 0, 0);
        QVERIFY(handler->addEvent(&e) != Event::UNASSIGNED_ID);
    }
    // Add 3 future events
    for (unsigned i=0; i<3; ++i){
        Event e("future"+QString::number(i),
                QDateTime::currentDateTime().addDays(-i).toString(Event::TIME_FORMAT),
                Event::STATIC, 0, 0);
        QVERIFY(handler->addEvent(&e) != Event::UNASSIGNED_ID);
    }

    std::vector<Event> events;

    QBENCHMARK {
        events = handler->nextEvents(QDateTime::currentDateTime().toString(Event::TIME_FORMAT), 3u);
    }
}


void DatabaseHandlerBenchmark::nextEventsBenchmark_data()
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
    handler->clearAll();

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


void DatabaseHandlerBenchmark::addThousandEvents()
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
    handler->clearAll();
    events_.clear();

    // Create events.
    currentTime_ = QDateTime::currentDateTime();
    for (int i=1; i<=1000; ++i){
        Event e;
        if (i%2 == 0){
            // Even id = Static expired.
            e = Event("name" + QString::number(i), currentTime_.addDays(-i).toString(Event::TIME_FORMAT),
                      Event::STATIC, i*1000, Event::INFINITE_REPEAT);
        }
        else {
            // Odd id = Dynamic future.
            e = Event("name" + QString::number(i), currentTime_.addDays(i).toString(Event::TIME_FORMAT),
                      Event::DYNAMIC, i*1000, Event::INFINITE_REPEAT);
        }
        events_.push_back(e);
    }
    QVERIFY(events_.size() == 1000);

    // Add events
    QBENCHMARK_ONCE {
        for (unsigned i=0; i<events_.size(); ++i) {
            handler->addEvent(&(events_[i]));
        }
    }

    // Check that events were added
    for (Event e : events_){
        QVERIFY(e.id() != Event::UNASSIGNED_ID);
    }
}


void DatabaseHandlerBenchmark::addThousandEvents_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::updateThousandEvents()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QVERIFY(events_.size() == 1000);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    // Create updated events.
    std::vector<Event> updated;
    for (Event e : events_){
        Event u;
        if (e.id() % 2 == 0){
            u = Event(e.name()+"_u",
                      QDateTime::fromString(e.timestamp(), Event::TIME_FORMAT).addDays(-1).toString(Event::TIME_FORMAT),
                      Event::DYNAMIC, e.interval()+1000, e.repeats()+1);
        }
        else {
            u = Event(e.name()+"_u",
                      QDateTime::fromString(e.timestamp(), Event::TIME_FORMAT).addDays(1).toString(Event::TIME_FORMAT),
                      Event::STATIC, e.interval()+1000, e.repeats()+1);
        }
        updated.push_back(u);
    }
    QVERIFY(updated.size() == 1000);

    // Update events
    QBENCHMARK_ONCE {
        for (unsigned i=0; i<events_.size(); ++i){
            QVERIFY(handler->updateEvent(events_[i].id(), updated[i]));
        }
    }
}


void DatabaseHandlerBenchmark::updateThousandEvents_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::getThousandEvents()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QVERIFY(events_.size() == 1000);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    // Update events_ -container.
    QBENCHMARK_ONCE {
        for (unsigned i=0; i<events_.size(); ++i){
            events_[i] = handler->getEvent(events_[i].id());
        }
    }

    // Verify that all events are right.
    for (Event e : events_) {
        QVERIFY(e.id() != Event::UNASSIGNED_ID);
    }
}


void DatabaseHandlerBenchmark::getThousandEvents_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::get500ExpiredEvents()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QVERIFY(events_.size() == 1000);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    // Get expired
    std::vector<Event> expired;
    QString timeStr = currentTime_.toString(Event::TIME_FORMAT);
    QBENCHMARK_ONCE{
        expired = handler->checkOccured(timeStr);
    }

    // Check that expired events are right.
    QCOMPARE(expired.size(), std::vector<Event>::size_type(500));
    for (Event e : expired){
        QVERIFY(e.id() != Event::UNASSIGNED_ID);
        QVERIFY(e.timestamp() < currentTime_.toString(Event::TIME_FORMAT));
        Event original = events_[e.id()-1];
        QCOMPARE(e.id(), original.id());
        QCOMPARE(e.name(), original.name());
        QCOMPARE(e.timestamp(), original.timestamp());
        QCOMPARE(e.type(), original.type());
        QCOMPARE(e.interval(), original.interval());
        QCOMPARE(e.repeats(), original.repeats());
    }
}


void DatabaseHandlerBenchmark::get500ExpiredEvents_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::oneNextEventsFromThousandEvents()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QVERIFY(events_.size() == 1000);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    QDateTime lastEventTime = QDateTime::fromString(handler->getEvent(999).timestamp(), Event::TIME_FORMAT);
    Event e("name1001", lastEventTime.addDays(1).toString(Event::TIME_FORMAT),
            Event::STATIC, 0, 0);
    handler->addEvent(&e);
    QString lastEventTimeStr = lastEventTime.toString(Event::TIME_FORMAT);

    QBENCHMARK {
        QCOMPARE(handler->nextEvents(lastEventTimeStr, 1).size(),
                 std::vector<Event>::size_type(1));
    }
}


void DatabaseHandlerBenchmark::oneNextEventsFromThousandEvents_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::thousandNextEvents()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QVERIFY(events_.size() == 1000);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    QDateTime firstEventTime = QDateTime::fromString(handler->getEvent(1000).timestamp(), Event::TIME_FORMAT);
    Event e("name1002", firstEventTime.addDays(1).toString(Event::TIME_FORMAT),
            Event::STATIC, 0, 0);
    handler->addEvent(&e);
    QString firstEventTimeStr = firstEventTime.toString(Event::TIME_FORMAT);

    QBENCHMARK {
        QCOMPARE(handler->nextEvents(firstEventTimeStr, 1000).size(),
                 std::vector<Event>::size_type(1000));
    }
}


void DatabaseHandlerBenchmark::thousandNextEvents_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::clear500DynamicEvents()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QVERIFY(events_.size() == 1000);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    QBENCHMARK_ONCE {
        QVERIFY(handler->clearDynamic());
    }

    // Verify that dynamic events were removed
    for (Event e : events_) {
        if (e.type() == Event::DYNAMIC){
            QCOMPARE(handler->getEvent(e.id()).id(), Event::UNASSIGNED_ID);
        }
        else {
            QCOMPARE(handler->getEvent(e.id()).id(), e.id());
        }
    }
}


void DatabaseHandlerBenchmark::clear500DynamicEvents_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::clear500Events()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QVERIFY(events_.size() == 1000);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    QBENCHMARK_ONCE {
        QVERIFY(handler->clearAll());
    }

    // Verify that all events are removed
    for (Event e : events_) {
        QCOMPARE(handler->getEvent(e.id()).id(), Event::UNASSIGNED_ID);
    }
}


void DatabaseHandlerBenchmark::clear500Events_data()
{
    constructorBenchmark_data();
}


void DatabaseHandlerBenchmark::removeThousandEvents()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QVERIFY(events_.size() == 1000);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->initDB(dbType, dbName, tableName, dbHost, userName, password);

    // Re-populate database.
    for (unsigned i=0; i<events_.size(); ++i){
        QVERIFY(handler->getEvent(events_[i].id()).id() == Event::UNASSIGNED_ID);
        Event e = events_[i].copy();
        QVERIFY(handler->addEvent(&e) != Event::UNASSIGNED_ID);
        QCOMPARE(e.id(), events_[i].id());
    }

    QBENCHMARK_ONCE {
        for (Event e : events_){
            QVERIFY(handler->removeEvent(e.id()));
        }
    }
}


void DatabaseHandlerBenchmark::removeThousandEvents_data()
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
    return h;
}


QTEST_APPLESS_MAIN(DatabaseHandlerBenchmark)

#include "tst_databasehandlerbenchmark.moc"
