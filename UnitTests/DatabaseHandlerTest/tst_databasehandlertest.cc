/**
 * @file
 * @brief Unit tests for the EventTimerNS::DatabaseHandler.
 * @author Perttu Paarlahti 2016.
 */

#include <QString>
#include <QtTest>
#include <QDateTime>
#include "databasehandler.hh"
#include <memory>


/**
 * @brief Unit tests for the DatabaseHandler class.
 */
class DatabaseHandlerTest : public QObject
{
    Q_OBJECT

public:
    DatabaseHandlerTest();

private Q_SLOTS:

    /**
     * @brief Test initializing dbHandler with unknown database type string.
     */
    void unknownDbTypeTest();

    /**
     * @brief Test initializing dbHandler with invalid table name.
     */
    void invalidTableNameTest();
    void invalidTableNameTest_data();

    /**
     * @brief Test getting event that does not exist.
     */
    void getEventNotFound();
    void getEventNotFound_data();

    /**
     * @brief Test adding events to the database.
     */
    void addEventsTest();
    void addEventsTest_data();

    /**
     * @brief Test populating database and removing events one by one.
     */
    void removeEventsTest();
    void removeEventsTest_data();

    /**
     * @brief Test clearing dynamic events.
     */
    void clearDynamicTest();
    void clearDynamicTest_data();

    /**
     * @brief Test updating events.
     */
    void updateEventTest();
    void updateEventTest_data();

    /**
     * @brief Test checking occured events.
     */
    void checkOccuredTest();
    void checkOccuredTest_data();

    /**
     * @brief Test the nextEvents method.
     */
    void nextEventsTest();
    void nextEventsTest_data();

    /**
     * @brief Test adding event and removing them imediately.
     */
    void addRemoveTest();
    void addRemoveTest_data();

    /**
     * @brief Test adding events and updating them imediately.
     */
    void addUpdateTest();
    void addUpdateTest_data();

    /**
     * @brief Test using simultaniously two handlers on different tables in same database.
     */
    void TwoHandlersDifferentTablesTest();
    void TwoHandlersDifferentTablesTest_data();

private:

    // Initialize database. Avoid boilerplate.
    std::shared_ptr<EventTimerNS::DatabaseHandler>
    setupDB(QString type, QString name, QString table, QString host, QString user, QString password);

    void verifyDbInitialization(std::shared_ptr<EventTimerNS::DatabaseHandler> h);

    // Verify that two events are identical
    void compareEvents(EventTimerNS::Event actual, EventTimerNS::Event expected);
};

DatabaseHandlerTest::DatabaseHandlerTest()
{
}


void DatabaseHandlerTest::unknownDbTypeTest()
{
    using namespace EventTimerNS;
    DatabaseHandler::DbSetup setup;
    setup.dbType = "Not_a_supported_db_driver";
    setup.dbName = "failDb";
    setup.tableName = "failTable";
    setup.dbHostName = QString();
    setup.userName = QString();
    setup.password = QString();

    DatabaseHandler h(setup);
    QVERIFY(!h.isValid());
}


void DatabaseHandlerTest::invalidTableNameTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    // 'invalidate' table name.
    tableName = "123" + tableName + "? i n v a l i d a t e d !";

    using namespace EventTimerNS;
    DatabaseHandler::DbSetup setup;
    setup.dbType = dbType;
    setup.dbName = dbName;
    setup.tableName = tableName;
    setup.dbHostName = dbHost;
    setup.userName = userName;
    setup.password = password;

    DatabaseHandler h(setup);
    QVERIFY(!h.isValid());
}


void DatabaseHandlerTest::invalidTableNameTest_data()
{
    this->addEventsTest_data();
}


void DatabaseHandlerTest::getEventNotFound()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    // Setup parameters.
    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->setupDB(dbType,dbName, tableName, dbHost, userName, password);

    // Clear db.
    QVERIFY( handler->clearAll() );
    Event e = handler->getEvent(1);
    QCOMPARE(e.id(), Event::UNASSIGNED_ID);
}


void DatabaseHandlerTest::getEventNotFound_data()
{
    addEventsTest_data();
}


void DatabaseHandlerTest::addEventsTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    // Setup parameters.
    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            this->setupDB(dbType,dbName, tableName, dbHost, userName, password);

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (unsigned i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        QDateTime time = current.addSecs(-i);
        QString timestamp = time.toString(Event::TIME_FORMAT);
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        unsigned id = handler->addEvent(&e);
        QCOMPARE(id, i);
        QCOMPARE(e.id(), i);
        events.push_back(e);
    }

    // Verify that database contains correct data.
    for (Event e : events) {
        Event e2 = handler->getEvent(e.id());
        this->compareEvents(e2, e);
    }
    QVERIFY(handler->clearAll());
}


void DatabaseHandlerTest::addEventsTest_data()
{
    QTest::addColumn<QString>("dbType");
    QTest::addColumn<QString>("dbName");
    QTest::addColumn<QString>("tableName");
    QTest::addColumn<QString>("dbHost");
    QTest::addColumn<QString>("userName");
    QTest::addColumn<QString>("password");

    // Running this test case requires SQLite3 to be installed.
    QTest::newRow("Local SQLite, no authentication")
            << "QSQLITE" << "testDB" << "events" << QString() << QString() << QString();

    //TODO: Other configurations
}


void DatabaseHandlerTest::removeEventsTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            setupDB(dbType, dbName, tableName, dbHost, userName, password);

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        QDateTime time = current.addSecs(-i);
        QString timestamp = time.toString(Event::TIME_FORMAT);
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        unsigned id = handler->addEvent(&e);
        QVERIFY(id != Event::UNASSIGNED_ID);
        events.push_back(e);
    }

    // Remove events one by one
    for (Event e : events){
        QVERIFY( handler->removeEvent(e.id()) );
        Event tmp = handler->getEvent(e.id());
        QCOMPARE(tmp.id(), Event::UNASSIGNED_ID);
        QVERIFY(handler->errorString().isEmpty());
    }
    QVERIFY(handler->clearAll());
}


void DatabaseHandlerTest::removeEventsTest_data()
{
    // Re-use addEvent test data.
    addEventsTest_data();
}


void DatabaseHandlerTest::clearDynamicTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            setupDB(dbType, dbName, tableName, dbHost, userName, password);

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        QDateTime time = current.addSecs(-i);
        QString timestamp = time.toString(Event::TIME_FORMAT);
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        int id = handler->addEvent(&e);
        QVERIFY(id != -1);
        events.push_back(e);
    }

    // Clear dynamic events and verify removal.
    QVERIFY(handler->clearDynamic());
    QVERIFY(handler->isValid());
    for (Event e : events) {
        Event tmp = handler->getEvent(e.id());
        if (e.type() == Event::DYNAMIC) {
            QCOMPARE(tmp.id(), Event::UNASSIGNED_ID);
            QVERIFY(handler->errorString().isEmpty());
            QVERIFY(handler->isValid());
        }
        else {
            this->compareEvents(tmp, e);
            QVERIFY(handler->isValid());
        }
    }
    QVERIFY(handler->clearAll());
}


void DatabaseHandlerTest::clearDynamicTest_data()
{
    addEventsTest_data();
}


void DatabaseHandlerTest::updateEventTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            setupDB(dbType, dbName, tableName, dbHost, userName, password);

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        QDateTime time = current.addSecs(-i);
        QString timestamp = time.toString(Event::TIME_FORMAT);
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        int id = handler->addEvent(&e);
        QVERIFY(id != -1);
        events.push_back(e);
    }

    // Update events
    for (Event e : events) {
        Event tmp = handler->getEvent(e.id());
        QVERIFY(tmp.id() != Event::UNASSIGNED_ID);
        QDateTime time = QDateTime::fromString(e.timestamp(), Event::TIME_FORMAT);
        Event replacement(e.name() + "_update",
                          time.addDays(1).toString(Event::TIME_FORMAT),
                          e.type() == Event::DYNAMIC ? Event::STATIC : Event::DYNAMIC,
                          e.interval() + 1000, e.repeats() + 1);

        QVERIFY(handler->updateEvent(e.id(), replacement));
        tmp = handler->getEvent(e.id());
        QCOMPARE(tmp.id(), e.id());
        QCOMPARE(tmp.name(), e.name() + "_update");
        QCOMPARE(tmp.interval(), e.interval() + 1000u);
        QCOMPARE(tmp.repeats(), e.repeats() + 1u);
        QVERIFY(tmp.type() != e.type());
        QCOMPARE(tmp.timestamp(),
                 QDateTime::fromString(e.timestamp(), Event::TIME_FORMAT).addDays(1).toString(Event::TIME_FORMAT));
    }
    QVERIFY(handler->clearAll());
}


void DatabaseHandlerTest::updateEventTest_data()
{
    addEventsTest_data();
}


void DatabaseHandlerTest::checkOccuredTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            setupDB(dbType, dbName, tableName, dbHost, userName, password);

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        int diff = i%2==0 ? -1000*i : 1000*i;
        QDateTime time = current.addSecs(diff);
        QString timestamp = time.toString(Event::TIME_FORMAT);
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        int id = handler->addEvent(&e);
        QVERIFY(id != -1);
        events.push_back(e);
    }

    std::vector<Event> occured = handler->checkOccured(current.toString(Event::TIME_FORMAT));
    QCOMPARE(occured.size(), std::vector<Event>::size_type(5));
    for (Event e : occured){
        QVERIFY(e.id()%2 == 0);
        QVERIFY(QDateTime::fromString(e.timestamp(), Event::TIME_FORMAT) < current);
        Event original = events.at(e.id()-1);
        this->compareEvents(e, original);
    }
    QVERIFY(handler->clearAll());
}


void DatabaseHandlerTest::checkOccuredTest_data()
{
    addEventsTest_data();
}


void DatabaseHandlerTest::nextEventsTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            setupDB(dbType, dbName, tableName, dbHost, userName, password);
    QString timeNow = QDateTime::currentDateTime().toString(Event::TIME_FORMAT);
    std::vector<Event> futureEvents;

    // Emty db produces empty list of next events without errors
    QCOMPARE(handler->nextEvents(QDateTime::currentDateTime().toString(Event::TIME_FORMAT), 100).size(),
             std::vector<Event>::size_type(0));
    QVERIFY(handler->errorString().isEmpty());
    QVERIFY(handler->isValid());

    // Add some events
    for (unsigned i=1; i<10; ++i){
        Event e("name" + QString::number(i),
                QDateTime::currentDateTime().addDays(i).toString(Event::TIME_FORMAT),
                i%2==0 ? Event::DYNAMIC : Event::STATIC,
                1000*i, i);
        QVERIFY(handler->addEvent(&e) != Event::UNASSIGNED_ID);
        QCOMPARE(e.id(), i);
        futureEvents.push_back(e);
    }

    // Add one passed event
    Event e("expired", QDateTime::currentDateTime().addDays(-1).toString(Event::TIME_FORMAT), Event::STATIC);
    QVERIFY(handler->addEvent(&e) != Event::UNASSIGNED_ID);

    // Verify that correct future events are in the result list, and expired event is not.
    for (unsigned i=1; i<futureEvents.size(); ++i){
        std::vector<Event> nextEvents = handler->nextEvents(timeNow, i);
        QVERIFY(handler->isValid());
        QVERIFY(handler->errorString().isEmpty());
        QCOMPARE(nextEvents.size(), std::vector<Event>::size_type(i));
        for (unsigned j=0; j<nextEvents.size(); ++j){
            this->compareEvents(nextEvents[j], futureEvents[j]);
        }
    }
}


void DatabaseHandlerTest::nextEventsTest_data()
{
    addEventsTest_data();
}


void DatabaseHandlerTest::addRemoveTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            setupDB(dbType, dbName, tableName, dbHost, userName, password);

    // AddEvents and remove imediately.
    QDateTime current = QDateTime::currentDateTime();
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        int diff = i%2==0 ? -1000*i : 1000*i;
        QDateTime time = current.addSecs(diff);
        QString timestamp = time.toString(Event::TIME_FORMAT);
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        // Add event
        unsigned id = handler->addEvent(&e);
        QVERIFY(id != Event::UNASSIGNED_ID);
        QCOMPARE(e.id(), id);
        Event tmp = handler->getEvent(id);
        this->compareEvents(tmp, e);

        // Remove event.
        QVERIFY(handler->removeEvent(id));
        tmp = handler->getEvent(id);
        QCOMPARE(tmp.id(), Event::UNASSIGNED_ID);
    }

}


void DatabaseHandlerTest::addRemoveTest_data()
{
    addEventsTest_data();
}


void DatabaseHandlerTest::addUpdateTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler =
            setupDB(dbType, dbName, tableName, dbHost, userName, password);

    // AddEvents and update imediately.
    QDateTime current = QDateTime::currentDateTime();
    for (unsigned i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        int diff = i%2==0 ? -1000*i : 1000*i;
        QDateTime time = current.addSecs(diff);
        QString timestamp = time.toString(Event::TIME_FORMAT);
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        // Add event
        unsigned id = handler->addEvent(&e);
        QVERIFY(id != Event::UNASSIGNED_ID);
        QCOMPARE(e.id(), id);
        Event tmp = handler->getEvent(id);
        this->compareEvents(tmp, e);

        // Update event.
        QDateTime updated_time = QDateTime::fromString(e.timestamp(), Event::TIME_FORMAT).addDays(i);
        Event updated(e.name()+"_upd", updated_time.toString(Event::TIME_FORMAT),
                      e.type() == Event::DYNAMIC ? Event::STATIC : Event::STATIC,
                      e.interval() + i*1000, e.repeats()+i);
        updated.setId(id);
        QVERIFY(handler->updateEvent(id, updated));
        tmp = handler->getEvent(id);
        this->compareEvents(tmp, updated);
    }
}


void DatabaseHandlerTest::addUpdateTest_data()
{
    this->addEventsTest_data();
}


void DatabaseHandlerTest::TwoHandlersDifferentTablesTest()
{
    QFETCH(QString, dbType);
    QFETCH(QString, dbName);
    QFETCH(QString, tableName);
    QFETCH(QString, dbHost);
    QFETCH(QString, userName);
    QFETCH(QString, password);
    QString tableName2 = tableName + "_2";

    using namespace EventTimerNS;
    std::shared_ptr<DatabaseHandler> handler1 =
            setupDB(dbType, dbName, tableName, dbHost, userName, password);
    std::shared_ptr<DatabaseHandler> handler2 =
            setupDB(dbType, dbName, tableName2, dbHost, userName, password);
    std::vector<std::shared_ptr<DatabaseHandler> > handlers = {handler1, handler2};

    // Add events
    std::vector<Event> events(20);
    QDateTime current = QDateTime::currentDateTime();
    for (int j = 1; j<11; ++j)
    {
        for (unsigned i = 0; i<handlers.size(); ++i) {
            Event e("name"+QString::number(10*i+j), current.addDays(10*i+1).toString(Event::TIME_FORMAT),
                    i % 2 == 0 ? Event::STATIC : Event::DYNAMIC, (i*10+j)*1000, i*10+j);

            unsigned id = handlers.at(i)->addEvent(&e);
            QVERIFY(id != Event::UNASSIGNED_ID);
            QCOMPARE(id, e.id());
            QVERIFY(handlers.at(i)->isValid());
            events[10*i+j-1] = e;
        }
    }

    // Check that events are in DBs.
    for (unsigned i=0; i<events.size(); ++i){
        Event expected = events.at(i);
        if (i < 10){
            Event actual = handler1->getEvent(expected.id());
            this->compareEvents(actual, expected);
        }
        else {
            Event actual = handler2->getEvent(expected.id());
            this->compareEvents(actual, expected);
        }
    }

    // Remove events.
    for (int j = 1; j<11; ++j)
    {
        for (unsigned i = 0; i<handlers.size(); ++i) {
            Event e = events.at(10*i+j-1);
            QVERIFY( handlers.at(i)->removeEvent(e.id()) );
        }
    }

    // Verify that all events are gone.
    for (Event e : events) {
        Event tmp = handler1->getEvent(e.id());
        QCOMPARE(tmp.id(), Event::UNASSIGNED_ID);
        tmp = handler2->getEvent(e.id());
        QCOMPARE(tmp.id(), Event::UNASSIGNED_ID);
    }
}


void DatabaseHandlerTest::TwoHandlersDifferentTablesTest_data()
{
    this->addEventsTest_data();
}



std::shared_ptr<EventTimerNS::DatabaseHandler>
DatabaseHandlerTest::setupDB(QString type, QString name, QString table, QString host, QString user, QString password)
{
    EventTimerNS::DatabaseHandler::DbSetup setup;
    setup.dbType = type;
    setup.dbName = name;
    setup.tableName = table;
    setup.dbHostName = host;
    setup.userName = user;
    setup.password = password;

    std::shared_ptr<EventTimerNS::DatabaseHandler> handler
            = std::shared_ptr<EventTimerNS::DatabaseHandler>(new EventTimerNS::DatabaseHandler(setup));

    verifyDbInitialization(handler);
    return handler;
}


void DatabaseHandlerTest::verifyDbInitialization(std::shared_ptr<EventTimerNS::DatabaseHandler> h)
{
    QVERIFY(h->isValid());
    QVERIFY(h->clearAll());
    QVERIFY(h->isValid());
}


void DatabaseHandlerTest::compareEvents(EventTimerNS::Event actual, EventTimerNS::Event expected)
{
    QCOMPARE (actual.id(), expected.id());
    QCOMPARE (actual.name(), expected.name());
    QCOMPARE (actual.timestamp(), expected.timestamp());
    QCOMPARE (actual.interval(), expected.interval());
    QCOMPARE (actual.repeats(), expected.repeats());
    QCOMPARE (actual.type(), expected.type());
}



QTEST_APPLESS_MAIN(DatabaseHandlerTest)

#include "tst_databasehandlertest.moc"
