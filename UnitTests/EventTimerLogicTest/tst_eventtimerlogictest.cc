#include <QString>
#include <QtTest>
#include "eventtimerbuilder.hh"


Q_DECLARE_METATYPE(EventTimerNS::EventTimerBuilder::Configuration)


class EventTimerLogicTest : public QObject
{
    Q_OBJECT

public:
    EventTimerLogicTest();

private Q_SLOTS:

    void validBuilderTest();
    void validBuilderTest_data();
};

EventTimerLogicTest::EventTimerLogicTest()
{
}


void EventTimerLogicTest::validBuilderTest()
{
    QFETCH(EventTimerNS::EventTimerBuilder::Configuration, conf);

    EventTimerNS::EventTimer* timer = EventTimerNS::EventTimerBuilder::create(conf);
    QVERIFY(timer->isValid());
    QCOMPARE(timer->errorString(), QString());
}


void EventTimerLogicTest::validBuilderTest_data()
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
        QTest::newRow("Local SQLite no authentication") << conf;
    }
}


QTEST_APPLESS_MAIN(EventTimerLogicTest)

#include "tst_eventtimerlogictest.moc"
