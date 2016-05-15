/**
 * @file
 * @brief Defines the Event class, which represents single event to be
 *  scheduled with EventTimer.
 * @author Perttu Paarlahti 2016.
 */

#ifndef EVENT_HH
#define EVENT_HH

#include <QString>

namespace EventTimerNS
{

/**
 * @brief This class represents single event to be stored in the database.
 */
class Event
{
public:

    /**
     * @brief Event's timestamp format string: 'yyyy-MM-dd hh:mm:ss:zzz'
     * (refer to QDateTime documentation).
     */
    static const QString TIME_FORMAT;

    /**
     * @brief Special value for repeats, indicating that event will be repeated infinite times.
     */
    static const unsigned INFINITE_REPEAT;

    /**
     * @brief Type of event. Static events are preserved in database between
     *  application runs, while dynamic events are removed at start-up.
     */
    enum Type{
        STATIC, DYNAMIC
    };

    /**
     * @brief Default constructor is implemented for conveniance.
     * @pre -
     * @post Constructs an invalid event (name and timestamp are empty strings).
     *  Do use such events with EventTimer.
     */
    Event();

    /**
     * @brief Constructor.
     * @param name Event's name.
     * @param timestamp Event's first time of occurence. Valid format:
     *  'dd-MM-yyyy hh:mm:ss:zzz', also defined by TIME_FORMAT.
     * @param type Event type (static or dynamic).
     * @param interval Time between repeated events (in milliseconds).
     *  Value 0 implies single-shot event.
     * @param repeats Number of repeats.
     * @pre timestamp is in a valid format. Name is not an empty string.
     *  If interval = 0, repeat should be 0 too.
     * @post Name, time, type, interval and repeats have been set.
     *  Event's id is -1 (unassigned).
     */
    Event(const QString& name,
          const QString& timestamp,
          Type type = DYNAMIC,
          unsigned interval = 0,
          unsigned repeats = 0);

    /**
     * @brief Destructor.
     */
    ~Event();

    /**
     * @brief Get event's name.
     * @return Name given in constructor.
     * @pre -
     */
    QString name() const;

    /**
     * @brief Get event's timestamp.
     * @return Timestamp given in constructor.
     * @pre -
     */
    QString timestamp() const;

    /**
     * @brief Get event's interval.
     * @return Interval given in constructor.
     * @pre -
     */
    unsigned interval() const;

    /**
     * @brief Get event's repeats.
     * @return Repeat times given in constructor.
     * @pre -
     */
    unsigned repeats() const;

    /**
     * @brief Get event's type.
     * @return Event type given in constructor.
     * @pre -
     */
    Type type() const;

    /**
     * @brief Get event's unique id.
     * @return Id's unique id set by the event timer. -1 means unassigned id.
     */
    int id() const;

    /**
     * @brief Assign event id.
     * @param id New id.
     * @pre Event timer calls this method automatically.
     *  Do not call this method explicitly. New id >= 0.
     */
    void setId(int id);

private:

    QString name_;
    QString timestamp_;
    unsigned interval_;
    unsigned repeats_;
    Type type_;
    int id_;
};

} // namespace EventTimerNS

#endif // EVENT_HH
