/**
 * @file
 * @brief Unit tests for the EventTimerNS::Event class.
 * @author Perttu Paarlahti 2016
 */

#include <QString>
#include <QtTest>
#include "event.hh"

// Introduce custom type to the qt meta object compilier.
Q_DECLARE_METATYPE(EventTimerNS::Event::Type)

/**
 * @brief Unit tests for the EventTimerNS::Event class.
 */
class EventTest : public QObject
{
    Q_OBJECT

public:
    EventTest();

private Q_SLOTS:

    /**
     * @brief Test Event constructor and setting id.
     */
    void eventTestCase();
    void eventTestCase_data();
};

EventTest::EventTest()
{
}


void EventTest::eventTestCase()
{
    QFETCH(QString, name);
    QFETCH(QString, timestamp);
    QFETCH(EventTimerNS::Event::Type, type);
    QFETCH(unsigned, interval);
    QFETCH(unsigned, repeats);
    QFETCH(int, id);

    // Test contructor
    EventTimerNS::Event e(name, timestamp, type, interval, repeats);
    QCOMPARE(e.name(), name);
    QCOMPARE(e.timestamp(), timestamp);
    QCOMPARE(e.type(), type);
    QCOMPARE(e.interval(), interval);
    QCOMPARE(e.repeats(), repeats);
    QCOMPARE(e.id(), -1);

    // Test setting id.
    e.setId(id);
    QCOMPARE(e.id(), id);
}


void EventTest::eventTestCase_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("timestamp");
    QTest::addColumn<EventTimerNS::Event::Type>("type");
    QTest::addColumn<unsigned>("interval");
    QTest::addColumn<unsigned>("repeats");
    QTest::addColumn<int>("id");

    QTest::newRow("cTest1") << "name1" << "2016-05-10 12:00:00:000" << EventTimerNS::Event::DYNAMIC << 0u    << 0u << 0;
    QTest::newRow("cTest2") << "name2" << "2016-05-10 12:00:30:000" << EventTimerNS::Event::STATIC  << 0u    << 0u << 1;
    QTest::newRow("cTest3") << "name3" << "2016-05-10 12:00:40:500" << EventTimerNS::Event::DYNAMIC << 4000u << 0u << 2;
    QTest::newRow("cTest4") << "name4" << "2016-05-10 12:00:00:000" << EventTimerNS::Event::STATIC  << 5000u << 3u << 3;
}



QTEST_APPLESS_MAIN(EventTest)

#include "tst_eventtest.moc"
