/**
 * @file
 * @brief Unit- and integration tests for the EventTimerLogic and
 *  EventTimerBuilder classes.
 * @author Perttu Paarlahti 2016.
 */

#include <QString>
#include <QtTest>
#include <memory>
#include "eventtimerbuilder.hh"

Q_DECLARE_METATYPE(EventTimerNS::EventTimerBuilder::Configuration)


/**
 * @brief Stub implementation for the Logger interface.
 */
class LoggerStub : public EventTimerNS::Logger
{
public:

    QStringList messages;

    void logMsg(const QString& msg)
    {
        messages.push_back(msg);
    }
};


/**
 * @brief Stub implementation for the EventHandler interface.
 */
class HandlerStub : public EventTimerNS::EventHandler
{
public:

    std::vector<EventTimerNS::Event> events;

    void notify(EventTimerNS::Event event)
    {
        events.push_back(event);
    }
};


/**
 * @brief Unit tests for the EventTimerLogic and EventTimerBuilder classes.
 */
class EventTimerLogicTest : public QObject
{
    Q_OBJECT

public:
    EventTimerLogicTest();

private Q_SLOTS:

    /**
     * @brief Test EventTimerBuilder with invalid configuration parameters.
     */
    void invalidBuildetTest();
    void invalidBuildetTest_data();

    /**
     * @brief Test EventTimerBuilder with valid configuration parameters.
     */
    void validBuilderTest();
    void validBuilderTest_data();

    /**
     * @brief Test adding new event.
     */
    void addEventTest();
    void addEventTest_data();

    /**
     * @brief Test removing event.
     */
    void removeEvent();
    void removeEvent_data();

    /**
     * @brief Test clearing dynamic events.
     */
    void clearDynamicTest();
    void clearDynamicTest_data();

    /**
     * @brief Test clearing all events.
     */
    void clearAllTest();
    void clearAllTest_data();

    /**
     * @brief Verify that starting the timer clears
     *  expired and dynamic events.
     */
    void startClearsDynamicAndExpiredEventsTest();
    void startClearsDynamicAndExpiredEventsTest_data();


private:

    void compareEvents(const EventTimerNS::Event& e1, const EventTimerNS::Event& e2) const;
};

EventTimerLogicTest::EventTimerLogicTest()
{
}


void EventTimerLogicTest::invalidBuildetTest()
{
    QFETCH(EventTimerNS::EventTimerBuilder::Configuration, conf);
    std::shared_ptr<EventTimerNS::EventTimer> timer;

    // Invalid dbType
    EventTimerNS::EventTimerBuilder::Configuration c2 = conf;
    c2.dbType = "Not a db type";
    timer.reset(EventTimerNS::EventTimerBuilder::create(c2));
    QVERIFY(!timer->isValid());
    QVERIFY(!timer->errorString().isEmpty());

    // Invalid table name.
    EventTimerNS::EventTimerBuilder::Configuration c3 = conf;
    c3.tableName = "123" + c3.tableName + "! i n v a l i d ?";
    timer.reset(EventTimerNS::EventTimerBuilder::create(c3));
    QVERIFY(!timer->isValid());
    QVERIFY(!timer->errorString().isEmpty());
}


void EventTimerLogicTest::invalidBuildetTest_data()
{
    QTest::addColumn<EventTimerNS::EventTimerBuilder::Configuration>("conf");

    {
        EventTimerNS::EventTimerBuilder::Configuration conf;
        conf.dbType = "QSQLITE";
        conf.dbName = "SQLiteTestDB";
        conf.tableName = "events";
        conf.dbHostName = QString();
        conf.userName = QString();
        conf.password = QString();
        conf.refreshRateMsec = 1000;
        QTest::newRow("Local SQLite no authentication") << conf;
    }
}


void EventTimerLogicTest::validBuilderTest()
{
    QFETCH(EventTimerNS::EventTimerBuilder::Configuration, conf);

    using namespace EventTimerNS;
    std::shared_ptr<EventTimer> timer(EventTimerBuilder::create(conf));
    QVERIFY(timer->isValid());
    QCOMPARE(timer->errorString(), QString());
}


void EventTimerLogicTest::validBuilderTest_data()
{
    invalidBuildetTest_data();
}


void EventTimerLogicTest::addEventTest()
{
    QFETCH(EventTimerNS::EventTimerBuilder::Configuration, conf);

    using namespace EventTimerNS;
    std::shared_ptr<EventTimer> timer(EventTimerBuilder::create(conf));
    LoggerStub logger;
    HandlerStub handler;
    timer->clearAll();
    timer->setLogger(&logger);
    timer->setEventHandler(&handler);

    // Add event
    Event e("name", "2000-01-01 00:00:00:000", Event::STATIC, 1000, 10);
    int id = timer->addEvent(&e);
    QCOMPARE(id, e.id());
    QVERIFY(id != -1);
    QCOMPARE(logger.messages.size(), QStringList::size_type(1));
    qDebug() << logger.messages.at(0);
    QCOMPARE(handler.events.size(), std::vector<Event>::size_type(0));
    QVERIFY(timer->isValid());
    QVERIFY(timer->errorString().isEmpty());

    // Verify addition
    Event e2 = timer->getEvent(e.id());
    QVERIFY(e2.id() != -1);
    QVERIFY(timer->isValid());
    QVERIFY(timer->errorString().isEmpty());
    QCOMPARE(logger.messages.size(), QStringList::size_type(1));
    QCOMPARE(handler.events.size(), std::vector<Event>::size_type(0));
    this->compareEvents(e, e2);
}


void EventTimerLogicTest::addEventTest_data()
{
    invalidBuildetTest_data();
}


void EventTimerLogicTest::removeEvent()
{
    QFETCH(EventTimerNS::EventTimerBuilder::Configuration, conf);

    using namespace EventTimerNS;
    std::shared_ptr<EventTimer> timer(EventTimerBuilder::create(conf));
    LoggerStub logger;
    HandlerStub handler;
    timer->clearAll();
    timer->setLogger(&logger);
    timer->setEventHandler(&handler);
    Event e("name", "2000-01-01 00:00:00:000", Event::STATIC, 1000, 10);
    timer->addEvent(&e);
    QVERIFY(e.id() != -1);

    // Remove existing event.
    QVERIFY(timer->removeEvent(e.id()));
    QCOMPARE(logger.messages.size(), QStringList::size_type(2));
    qDebug() << logger.messages.at(1);
    QCOMPARE(handler.events.size(), std::vector<Event>::size_type(0));
    QVERIFY(timer->isValid());
    QVERIFY(timer->errorString().isEmpty());

    // Verify removal
    Event tmp = timer->getEvent(e.id());
    QCOMPARE(tmp.id(), -1);
    QVERIFY(timer->isValid());
    QVERIFY(timer->errorString().isEmpty());
    QCOMPARE(logger.messages.size(), QStringList::size_type(3));
    qDebug() << logger.messages.at(2);
    QCOMPARE(handler.events.size(), std::vector<Event>::size_type(0));

    // Remove non-existing event.
    QVERIFY(timer->removeEvent(e.id()));
    QVERIFY(timer->isValid());
    QVERIFY(timer->errorString().isEmpty());
    QCOMPARE(logger.messages.size(), QStringList::size_type(4));
    qDebug() << logger.messages.at(3);
    QCOMPARE(handler.events.size(), std::vector<Event>::size_type(0));
}


void EventTimerLogicTest::removeEvent_data()
{
    invalidBuildetTest_data();
}


void EventTimerLogicTest::clearDynamicTest()
{
    QFETCH(EventTimerNS::EventTimerBuilder::Configuration, conf);

    using namespace EventTimerNS;
    std::shared_ptr<EventTimer> timer(EventTimerBuilder::create(conf));
    LoggerStub logger;
    HandlerStub handler;
    timer->clearAll();
    timer->setLogger(&logger);
    timer->setEventHandler(&handler);

    // Populate database.
    std::vector<Event> events;
    QDateTime currentTime = QDateTime::currentDateTime();
    for (int i=0; i<10; ++i){
        Event e("name"+QString::number(i+1),
                currentTime.addDays(i+1).toString(Event::TIME_FORMAT),
                i%2 == 0 ? Event::DYNAMIC : Event::STATIC,
                1000*(1+i), i+1);
        QVERIFY(timer->addEvent(&e) != -1);
        events.push_back(e);
    }

    // Clear dynamic events.
    QVERIFY(timer->clearDynamic());
    QCOMPARE(logger.messages.size(), QStringList::size_type(11));
    qDebug() << logger.messages.at(10);
    QCOMPARE(handler.events.size(), std::vector<Event>::size_type(0));

    // Verify that dynamic events are gone.
    for (Event e : events){
        Event e2 = timer->getEvent(e.id());
        if (e.type() == Event::STATIC) {
            this->compareEvents(e2, e);
        }
        else {
            QCOMPARE(e2.id(), -1);
        }
    }
    QCOMPARE(logger.messages.size(), QStringList::size_type(16));
    qDebug() << logger.messages.at(15);
    QCOMPARE(handler.events.size(), std::vector<Event>::size_type(0));
    QVERIFY(timer->isValid());
    QVERIFY(timer->errorString().isEmpty());
}


void EventTimerLogicTest::clearDynamicTest_data()
{
    invalidBuildetTest_data();
}


void EventTimerLogicTest::clearAllTest()
{
    QFETCH(EventTimerNS::EventTimerBuilder::Configuration, conf);

    using namespace EventTimerNS;
    std::shared_ptr<EventTimer> timer(EventTimerBuilder::create(conf));
    LoggerStub logger;
    HandlerStub handler;
    timer->clearAll();
    timer->setEventHandler(&handler);

    // Populate database.
    std::vector<Event> events;
    QDateTime currentTime = QDateTime::currentDateTime();
    for (int i=0; i<10; ++i){
        Event e("name"+QString::number(i+1),
                currentTime.addDays(i+1).toString(Event::TIME_FORMAT),
                i%2 == 0 ? Event::DYNAMIC : Event::STATIC,
                1000*(1+i), i+1);
        QVERIFY(timer->addEvent(&e) != -1);
        events.push_back(e);
    }

    timer->setLogger(&logger);
    QVERIFY(timer->clearAll());
    QCOMPARE(logger.messages.size(), QStringList::size_type(1));
    qDebug() << logger.messages.at(0);
    QCOMPARE(handler.events.size(), std::vector<Event>::size_type(0));
    QVERIFY(timer->isValid());
    QVERIFY(timer->errorString().isEmpty());

    for (Event e : events){
        QCOMPARE(timer->getEvent(e.id()).id(), -1);
    }
}


void EventTimerLogicTest::clearAllTest_data()
{
    invalidBuildetTest_data();
}


void EventTimerLogicTest::startClearsDynamicAndExpiredEventsTest()
{
    QFETCH(EventTimerNS::EventTimerBuilder::Configuration, conf);

    using namespace EventTimerNS;
    std::shared_ptr<EventTimer> timer (EventTimerBuilder::create(conf));
    HandlerStub handler;
    timer->clearAll();
    timer->setEventHandler(&handler);

    // Add some events.
    std::vector<Event> events;
    QDateTime current = QDateTime::currentDateTime();
    for (int i=0; i<16; ++i){
        Event e;
        if (i%4 == 0){
            // Dynamic event.
            e = Event("name"+QString::number(i+1),
                      current.addDays(i+1).toString(Event::TIME_FORMAT),
                      Event::DYNAMIC, 1000*i, i);
        }
        else if (i%4 == 1) {
            // Expired event
            e = Event("name"+QString::number(i+1),
                      current.addDays(-(i+1)).toString(Event::TIME_FORMAT),
                      Event::STATIC, 1000*i, i);
        }
        else if (i%4 == 2){
            // Expired event that can be updated.
            e = Event("name" + QString::number(i+1),
                      current.addDays(-(i+1)).toString(Event::TIME_FORMAT),
                      Event::STATIC, 24*60*60*1000, i+2);
        }
        else {
            // Static, future event
            e = Event("name"+QString::number(i+1),
                      current.addDays(i+1).toString(Event::TIME_FORMAT),
                      Event::STATIC, 1000*i, i);
        }
        QVERIFY (timer->addEvent(&e) != -1);
        events.push_back(e);
    }

    // Start should clean expired and dynamic events.
    timer->start();
    timer->stop();
    QVERIFY(timer->isValid());
    QVERIFY(timer->errorString().isEmpty());

    // Verify that expired and dynamic events are gone.
    for (unsigned i=0; i<events.size(); ++i){
        Event e = events.at(i);
        if (i%4 == 2){
            Event actual = timer->getEvent(e.id());
            QCOMPARE(actual.id(), e.id());
            QCOMPARE(QDateTime::fromString(actual.timestamp(), Event::TIME_FORMAT),
                     current.addDays(1));
            QCOMPARE(actual.type(), Event::STATIC);
            QCOMPARE(actual.interval(), e.interval());
            QCOMPARE(actual.repeats(), 0u);
        }
        else if (i%4 == 3){
            Event actual = timer->getEvent(e.id());
            this->compareEvents(actual, e);
        }
        else {
            QCOMPARE(timer->getEvent(e.id()).id(), -1);
        }
    }
}


void EventTimerLogicTest::startClearsDynamicAndExpiredEventsTest_data()
{
    invalidBuildetTest_data();
}


void EventTimerLogicTest::compareEvents(const EventTimerNS::Event& e1,
                                        const EventTimerNS::Event& e2) const
{
    QCOMPARE(e1.id(), e2.id());
    QCOMPARE(e1.name(), e2.name());
    QCOMPARE(e1.timestamp(), e2.timestamp());
    QCOMPARE(e1.type(), e2.type());
    QCOMPARE(e1.interval(), e2.interval());
    QCOMPARE(e1.repeats(), e2.repeats());
}


QTEST_APPLESS_MAIN(EventTimerLogicTest)

#include "tst_eventtimerlogictest.moc"
