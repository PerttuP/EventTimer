/**
 * @file
 * @brief Simple example program for using the EventTimer component.
 *  REQUIREMENTS: SQLite installed.
 * @author Perttu Paarlahti 2016.
 */

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <memory>
#include "eventtimerbuilder.hh"

using namespace EventTimerNS;

/**
 * @brief Implemets the EventTimer::Logger interface.
 *  Simply prints messages to stdout.
 */
class MyLogger : public Logger
{
public:
    void logMsg(const QString& msg)
    {
        qDebug() << "Log message:" << msg;
    }
};


/**
 * @brief Implements the EventTimerNS::EventHandler interface.
 *  Prints current time, event name, id and remaining repeats to the stdout.
 */
class MyHandler : public EventHandler
{
public:
    void notify(const Event& event)
    {
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss").toStdString().c_str()
                 << "id:" << event.id() << "name:" << event.name()
                 << "repeats left:" << event.repeats();

        if (event.name() == "quitEvent"){
            QCoreApplication::exit(0);
        }
    }
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create EventTimer.
    EventTimerBuilder::Configuration conf;
    conf.dbType = "QSQLITE";
    conf.dbName = "demoDB";
    conf.tableName = "events";
    conf.refreshRateMsec = 500;
    std::unique_ptr<EventTimer> timer(EventTimerBuilder::create(conf));
    timer->clearAll();
    MyHandler handler;
    MyLogger logger;
    timer->setEventHandler(&handler);
    timer->setLogger(&logger);

    // Add static events with different frequency and repeat times.
    QDateTime now = QDateTime::currentDateTime();
    Event e1("fooEvent", now.addSecs(1).toString(Event::TIME_FORMAT),
             Event::STATIC, 1000, 9);
    timer->addEvent(&e1);

    Event e2("barEvent", now.addSecs(1).toString(Event::TIME_FORMAT),
             Event::STATIC, 2000, 4);
    timer->addEvent(&e2);

    // Infinite event
    Event e3("infEvent", now.addSecs(1).toString(Event::TIME_FORMAT),
             Event::STATIC, 1000, Event::INFINITE_REPEAT);
    timer->addEvent(&e3);

    // Start polling events.
    timer->start();

    // Dynamic, single time event:
    Event e4("quitEvent", now.addSecs(10).toString(Event::TIME_FORMAT), Event::DYNAMIC);
    timer->addEvent(&e4);

    return a.exec();
}


/* Possible output:
 *
 * Log message: "Event added. Id = 1"
 * Log message: "Event added. Id = 2"
 * Log message: "Event added. Id = 3"
 * Log message: "Dynamic events cleared successfully."
 * Log message: "Timer started."
 * Log message: "Event added. Id = 4"
 * 12:00:00 id: 1 name: "fooEvent" repeats left: 9
 * 12:00:00 id: 2 name: "barEvent" repeats left: 4
 * 12:00:00 id: 3 name: "infEvent" repeats left: 4294967295
 * 12:00:01 id: 1 name: "fooEvent" repeats left: 8
 * 12:00:01 id: 3 name: "infEvent" repeats left: 4294967295
 * 12:00:02 id: 1 name: "fooEvent" repeats left: 7
 * 12:00:02 id: 2 name: "barEvent" repeats left: 3
 * 12:00:02 id: 3 name: "infEvent" repeats left: 4294967295
 * 12:00:03 id: 1 name: "fooEvent" repeats left: 6
 * 12:00:03 id: 3 name: "infEvent" repeats left: 4294967295
 * 12:00:04 id: 1 name: "fooEvent" repeats left: 5
 * 12:00:04 id: 2 name: "barEvent" repeats left: 2
 * 12:00:04 id: 3 name: "infEvent" repeats left: 4294967295
 * 12:00:05 id: 1 name: "fooEvent" repeats left: 4
 * 12:00:05 id: 3 name: "infEvent" repeats left: 4294967295
 * 12:00:06 id: 1 name: "fooEvent" repeats left: 3
 * 12:00:06 id: 2 name: "barEvent" repeats left: 1
 * 12:00:06 id: 3 name: "infEvent" repeats left: 4294967295
 * 12:00:07 id: 1 name: "fooEvent" repeats left: 2
 * 12:00:07 id: 3 name: "infEvent" repeats left: 4294967295
 * Log message: "Event removed (id = 2)."
 * 12:00:08 id: 1 name: "fooEvent" repeats left: 1
 * 12:00:08 id: 2 name: "barEvent" repeats left: 0
 * 12:00:08 id: 3 name: "infEvent" repeats left: 4294967295
 * Log message: "Event removed (id = 1)."
 * Log message: "Event removed (id = 4)."
 * 12:00:09 id: 1 name: "fooEvent" repeats left: 0
 * 12:00:09 id: 3 name: "infEvent" repeats left: 4294967295
 * 12:00:09 id: 4 name: "quitEvent" repeats left: 0
 *
 */
