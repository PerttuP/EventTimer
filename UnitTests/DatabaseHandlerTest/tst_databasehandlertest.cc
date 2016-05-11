/**
 * @file
 * @brief Unit tests for the EventTimerNS::DatabaseHandler.
 * @author Perttu Paarlahti 2016.
 */

#include <QString>
#include <QtTest>
#include <QDateTime>
#include "databasehandler.hh"


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
     * @brief Test adding events to the database.
     */
    void addEventsTest();
    void addEventsTest_data();

    /**
     * @brief Test removing events from the database.
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
};

DatabaseHandlerTest::DatabaseHandlerTest()
{
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
    DatabaseHandler::DbSetup setup;
    setup.dbType = dbType;
    setup.dbName = dbName;
    setup.tableName = tableName;
    setup.dbHostName = dbHost;
    setup.userName = userName;
    setup.password = password;

    // Open and clear database.
    DatabaseHandler handler(setup);
    QVERIFY2(handler.isValid(), "Initializing database failed");
    QVERIFY(handler.clearAll());
    QVERIFY(handler.isValid());

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        QDateTime time = current.addSecs(-i);
        QString timestamp = time.toString("yyyy-MM-dd hh:mm:ss:zzz");
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        int id = handler.addEvent(&e);
        QCOMPARE(id, i);
        QCOMPARE(e.id(), i);
        events.push_back(e);
    }

    // Verify that database contains correct data.
    for (Event e : events) {
        Event e2 = handler.getEvent(e.id());
        QCOMPARE(e2.id(), e.id());
        QCOMPARE(e2.name(), e.name());
        QCOMPARE(e2.timestamp(), e.timestamp());
        QCOMPARE(e2.interval(), e.interval());
        QCOMPARE(e2.repeats(), e.repeats());
        QCOMPARE(e2.type(), e.type());
    }
    QVERIFY(handler.clearAll());
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

    // Setup parameters.
    using namespace EventTimerNS;
    DatabaseHandler::DbSetup setup;
    setup.dbType = dbType;
    setup.dbName = dbName;
    setup.tableName = tableName;
    setup.dbHostName = dbHost;
    setup.userName = userName;
    setup.password = password;

    // Open and clear database.
    DatabaseHandler handler(setup);
    QVERIFY2(handler.isValid(), "Initializing database failed");
    QVERIFY(handler.clearAll());
    QVERIFY(handler.isValid());

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        QDateTime time = current.addSecs(-i);
        QString timestamp = time.toString("yyyy-MM-dd hh:mm:ss:zzz");
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        int id = handler.addEvent(&e);
        QVERIFY(id != -1);
        events.push_back(e);
    }

    // Remove events one by one
    for (Event e : events){
        QVERIFY( handler.removeEvent(e.id()) );
        Event tmp = handler.getEvent(e.id());
        QCOMPARE(tmp.id(), -1);
        QVERIFY(handler.errorString().isEmpty());
    }
    QVERIFY(handler.clearAll());
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

    // Setup parameters.
    using namespace EventTimerNS;
    DatabaseHandler::DbSetup setup;
    setup.dbType = dbType;
    setup.dbName = dbName;
    setup.tableName = tableName;
    setup.dbHostName = dbHost;
    setup.userName = userName;
    setup.password = password;

    // Open and clear database.
    DatabaseHandler handler(setup);
    QVERIFY2(handler.isValid(), "Initializing database failed");
    QVERIFY(handler.clearAll());
    QVERIFY(handler.isValid());

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        QDateTime time = current.addSecs(-i);
        QString timestamp = time.toString("yyyy-MM-dd hh:mm:ss:zzz");
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        int id = handler.addEvent(&e);
        QVERIFY(id != -1);
        events.push_back(e);
    }

    // Clear dynamic events and verify removal.
    QVERIFY(handler.clearDynamic());
    QVERIFY(handler.isValid());
    for (Event e : events) {
        Event tmp = handler.getEvent(e.id());
        if (e.type() == Event::DYNAMIC) {
            QCOMPARE(tmp.id(), -1);
            QVERIFY(handler.errorString().isEmpty());
            QVERIFY(handler.isValid());
        }
        else {
            QCOMPARE(tmp.id(), e.id());
            QCOMPARE(tmp.name(), e.name());
            QCOMPARE(tmp.timestamp(), e.timestamp());
            QCOMPARE(tmp.interval(), e.interval());
            QCOMPARE(tmp.repeats(), e.repeats());
            QCOMPARE(tmp.type(), e.type());
            QVERIFY(handler.isValid());
        }
    }
    QVERIFY(handler.clearAll());
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

    // Setup parameters.
    using namespace EventTimerNS;
    DatabaseHandler::DbSetup setup;
    setup.dbType = dbType;
    setup.dbName = dbName;
    setup.tableName = tableName;
    setup.dbHostName = dbHost;
    setup.userName = userName;
    setup.password = password;

    // Open and clear database.
    DatabaseHandler handler(setup);
    QVERIFY2(handler.isValid(), "Initializing database failed");
    QVERIFY(handler.clearAll());
    QVERIFY(handler.isValid());

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        QDateTime time = current.addSecs(-i);
        QString timestamp = time.toString("yyyy-MM-dd hh:mm:ss:zzz");
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        int id = handler.addEvent(&e);
        QVERIFY(id != -1);
        events.push_back(e);
    }

    // Update events
    for (Event e : events) {
        Event tmp = handler.getEvent(e.id());
        QVERIFY(tmp.id() != -1);
        QDateTime time = QDateTime::fromString(e.timestamp(), "yyyy-MM-dd hh:mm:ss:zzz");
        Event replacement(e.name() + "_update",
                          time.addDays(1).toString("yyyy-MM-dd hh:mm:ss:zzz"),
                          e.type() == Event::DYNAMIC ? Event::STATIC : Event::DYNAMIC,
                          e.interval() + 1000, e.repeats() + 1);

        QVERIFY(handler.updateEvent(e.id(), replacement));
        tmp = handler.getEvent(e.id());
        QCOMPARE(tmp.id(), e.id());
        QCOMPARE(tmp.name(), e.name() + "_update");
        QCOMPARE(tmp.interval(), e.interval() + 1000u);
        QCOMPARE(tmp.repeats(), e.repeats() + 1u);
        QVERIFY(tmp.type() != e.type());
        QCOMPARE(tmp.timestamp(),
                 QDateTime::fromString(e.timestamp(), "yyyy-MM-dd hh:mm:ss:zzz").addDays(1).toString("yyyy-MM-dd hh:mm:ss:zzz"));
    }
    QVERIFY(handler.clearAll());
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

    // Setup parameters.
    using namespace EventTimerNS;
    DatabaseHandler::DbSetup setup;
    setup.dbType = dbType;
    setup.dbName = dbName;
    setup.tableName = tableName;
    setup.dbHostName = dbHost;
    setup.userName = userName;
    setup.password = password;

    // Open and clear database.
    DatabaseHandler handler(setup);
    QVERIFY2(handler.isValid(), "Initializing database failed");
    QVERIFY(handler.clearAll());
    QVERIFY(handler.isValid());

    // Populate database.
    QDateTime current = QDateTime::currentDateTime();
    std::vector<Event> events;
    for (int i=1; i<11; ++i){
        QString name = "name" + QString::number(i);
        int diff = i%2==0 ? -1000*i : 1000*i;
        QDateTime time = current.addSecs(diff);
        QString timestamp = time.toString("yyyy-MM-dd hh:mm:ss:zzz");
        Event::Type type = i%2 == 0 ? Event::STATIC : Event::DYNAMIC;
        Event e(name, timestamp, type, i, i);

        int id = handler.addEvent(&e);
        QVERIFY(id != -1);
        events.push_back(e);
    }

    std::vector<Event> occured = handler.checkOccured(current);
    QCOMPARE(occured.size(), std::vector<Event>::size_type(5));
    for (Event e : occured){
        QVERIFY(e.id()%2 == 0);
        QVERIFY(QDateTime::fromString(e.timestamp(), "yyyy-MM-dd hh:mm:ss:zzz") < current);
        Event original = events.at(e.id()-1);
        QCOMPARE(e.name(), original.name());
        QCOMPARE(e.timestamp(), original.timestamp());
        QCOMPARE(e.interval(), original.interval());
        QCOMPARE(e.repeats(), original.repeats());
        QCOMPARE(e.type(), original.type());
    }
    QVERIFY(handler.clearAll());
}


void DatabaseHandlerTest::checkOccuredTest_data()
{
    addEventsTest_data();
}



QTEST_APPLESS_MAIN(DatabaseHandlerTest)

#include "tst_databasehandlertest.moc"
