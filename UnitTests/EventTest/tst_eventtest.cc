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
    void constructorTest();
    void constructorTest_data();

    /**
     * @brief Test the isValid-method and setter-methods.
     */
    void isValidTest();
    void isValidTest_data();

    /**
     * @brief Test the copy method.
     */
    void copyTest();
    void copyTest_data();
};

EventTest::EventTest()
{
}


void EventTest::constructorTest()
{
    QFETCH(QString, name);
    QFETCH(QString, timestamp);
    QFETCH(EventTimerNS::Event::Type, type);
    QFETCH(unsigned, interval);
    QFETCH(unsigned, repeats);
    QFETCH(unsigned, id);

    // Test contructor
    EventTimerNS::Event e(name, timestamp, type, interval, repeats);
    QCOMPARE(e.name(), name);
    QCOMPARE(e.timestamp(), timestamp);
    QCOMPARE(e.type(), type);
    QCOMPARE(e.interval(), interval);
    QCOMPARE(e.repeats(), repeats);
    QCOMPARE(e.id(), EventTimerNS::Event::UNASSIGNED_ID);
    QVERIFY(e.isValid());

    // Test setting id.
    e.setId(id);
    QCOMPARE(e.id(), id);
}


void EventTest::constructorTest_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("timestamp");
    QTest::addColumn<EventTimerNS::Event::Type>("type");
    QTest::addColumn<unsigned>("interval");
    QTest::addColumn<unsigned>("repeats");
    QTest::addColumn<unsigned>("id");

    QTest::newRow("cTest1") << "name1" << "2016-05-10 12:00:00:000" << EventTimerNS::Event::DYNAMIC << 0u    << 0u << 0u;
    QTest::newRow("cTest2") << "name2" << "2016-05-10 12:00:30:000" << EventTimerNS::Event::STATIC  << 0u    << 0u << 1u;
    QTest::newRow("cTest3") << "name3" << "2016-05-10 12:00:40:500" << EventTimerNS::Event::DYNAMIC << 4000u << 0u << 2u;
    QTest::newRow("cTest4") << "name4" << "2016-05-10 12:00:00:000" << EventTimerNS::Event::STATIC  << 5000u << 3u << 3u;
}


void EventTest::isValidTest()
{
    QFETCH(QString, name);
    QFETCH(QString, timestamp);
    QFETCH(EventTimerNS::Event::Type, type);
    QFETCH(unsigned, interval);
    QFETCH(unsigned, repeats);
    QFETCH(bool, isValid);

    EventTimerNS::Event e;
    QVERIFY(!e.isValid());

    e.setName(name);
    QCOMPARE(e.name(), name);
    QVERIFY(!e.isValid());

    e.setTimestamp(timestamp);
    QCOMPARE(e.timestamp(), timestamp);

    e.setType(type);
    QCOMPARE(e.type(), type);

    e.setInterval(interval);
    QCOMPARE(e.interval(), interval);

    e.setRepeats(repeats);
    QCOMPARE(e.repeats(), repeats);

    QCOMPARE(e.isValid(), isValid);
}


void EventTest::isValidTest_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("timestamp");
    QTest::addColumn<EventTimerNS::Event::Type>("type");
    QTest::addColumn<unsigned>("interval");
    QTest::addColumn<unsigned>("repeats");
    QTest::addColumn<bool>("isValid");

    QTest::newRow("static single time")      << "name1" << "2016-05-17 06:10:10:100" << EventTimerNS::Event::STATIC  << 0u     << 0u << true;
    QTest::newRow("static 1 remaining")      << "name2" << "2016-05-16 12:20:20:200" << EventTimerNS::Event::STATIC  << 1000u  << 0u << true;
    QTest::newRow("static 5 remaining")      << "name3" << "2016-05-15 18:30:30:300" << EventTimerNS::Event::STATIC  << 2000u  << 4u << true;

    QTest::newRow("dynamic single time")     << "name4" << "2016-05-14 00:01:01:010" << EventTimerNS::Event::DYNAMIC << 0u     << 0u << true;
    QTest::newRow("dynamic 1 remaining")     << "name5" << "2016-05-13 09:02:02:020" << EventTimerNS::Event::DYNAMIC << 300u   << 0u << true;
    QTest::newRow("dynamic 5 remaining")     << "name6" << "2016-05-12 15:03:03:030" << EventTimerNS::Event::DYNAMIC << 40000u << 4u << true;

    QTest::newRow("empty name")              << ""      << "2016-05-17 06:10:10:100" << EventTimerNS::Event::STATIC  << 0u     << 0u << false;
    QTest::newRow("wrong time format")       << "name7" << "2016-05-17 06:00:00"     << EventTimerNS::Event::DYNAMIC << 0u     << 0u << false;
    QTest::newRow("invalid timestamp")       << "name8" << "2016-17-05 06:00:00:000" << EventTimerNS::Event::STATIC  << 0u     << 0u << false;
    QTest::newRow("interval 0, repeats not") << "name9" << "2016-05-17 06:10:10:100" << EventTimerNS::Event::DYNAMIC << 0u     << 4u << false;
}


void EventTest::copyTest()
{
    QFETCH(QString, name);
    QFETCH(QString, timestamp);
    QFETCH(EventTimerNS::Event::Type, type);
    QFETCH(unsigned, interval);
    QFETCH(unsigned, repeats);
    QFETCH(unsigned, id);

    EventTimerNS::Event e1(name, timestamp, type, interval, repeats);

    // Copy event with unassigned id.
    EventTimerNS::Event e2 = e1.copy();
    QCOMPARE(e2.name(), e1.name());
    QCOMPARE(e2.timestamp(), e1.timestamp());
    QCOMPARE(e2.type(), e1.type());
    QCOMPARE(e2.interval(), e1.interval());
    QCOMPARE(e2.repeats(), e1.repeats());
    QCOMPARE(e2.id(), EventTimerNS::Event::UNASSIGNED_ID);
    QCOMPARE(e1.id(), EventTimerNS::Event::UNASSIGNED_ID);

    // Copy event with an assigned id.
    e1.setId(id);
    EventTimerNS::Event e3 = e1.copy();
    QCOMPARE(e3.name(), e1.name());
    QCOMPARE(e3.timestamp(), e1.timestamp());
    QCOMPARE(e3.type(), e1.type());
    QCOMPARE(e3.interval(), e1.interval());
    QCOMPARE(e3.repeats(), e1.repeats());
    QCOMPARE(e3.id(), EventTimerNS::Event::UNASSIGNED_ID);
    QCOMPARE(e1.id(), id);
}


void EventTest::copyTest_data()
{
    // Reuse test data from constructor test.
    constructorTest_data();
}



QTEST_APPLESS_MAIN(EventTest)

#include "tst_eventtest.moc"
