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
     * @brief Special value for unassigned event id.
     */
    static const unsigned UNASSIGNED_ID;

    /**
     * @brief Type of event. Static events are preserved in database between
     *  application runs, while dynamic events are removed at start-up.
     */
    enum Type{
        STATIC, DYNAMIC
    };

    /**
     * @brief Default constructor is implemented for convenience.
     * @pre -
     * @post Constructs an invalid event.
     *  Do not use such events with EventTimer.
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
     *  Event is in a valid state. Event's id is unassigned.
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
     * @brief Create new event having same values as the original (except for id).
     * @return New event having same name, timestamp, type, interval and repeats as the original.
     *  New event's id is unassigned.
     */
    Event copy() const;

    /**
     * @brief Get event's name.
     * @return Name given in constructor.
     * @pre -
     */
    QString name() const;

    /**
     * @brief Set new name for the event.
     * @param name New name.
     * @pre -
     * @post New name has been set.
     */
    void setName(const QString& name);

    /**
     * @brief Get event's timestamp.
     * @return Timestamp given in constructor.
     * @pre -
     */
    QString timestamp() const;

    /**
     * @brief Set new timestamp for the event.
     * @param timestamp New timestamp.
     * @pre -
     * @post New timestamp has been set.
     */
    void setTimestamp(const QString& timestamp);

    /**
     * @brief Get event's interval.
     * @return Interval given in constructor.
     * @pre -
     */
    unsigned interval() const;

    /**
     * @brief Set new interval for the event.
     * @param interval New interval.
     * @pre -
     * @post New interval has been set.
     */
    void setInterval(unsigned interval);

    /**
     * @brief Get event's repeats.
     * @return Repeat times given in constructor.
     * @pre -
     */
    unsigned repeats() const;

    /**
     * @brief Set new repeat times for the event.
     * @param repeats New number of repeats.
     * @pre -
     * @post New repeat count has been set.
     */
    void setRepeats(unsigned repeats);

    /**
     * @brief Get event's type.
     * @return Event type given in constructor.
     * @pre -
     */
    Type type() const;

    /**
     * @brief Set new type for the event.
     * @param type New event type.
     * @pre -
     * @post New event type has been set.
     */
    void setType(Type type);

    /**
     * @brief Get event's unique id.
     * @return Id's unique id set by the event timer.
     *  Returns Event::UNASSIGNED_ID, if id is not assigned yet.
     */
    unsigned id() const;

    /**
     * @brief Assign event id.
     * @param id New id.
     * @pre Event timer calls this method automatically.
     *  Do not call this method explicitly.
     * @post Event id is set.
     */
    void setId(unsigned id);

    /**
     * @brief Check if the event is in a valid state.
     * @return True, if event is in a valid state:
     *  1) Name is not an empty string.
     *  2) Timestamp is in the right format (TIME_FORMAT).
     *  3) If interval is 0, then repeats is 0 too.
     */
    bool isValid() const;


private:

    QString name_;
    QString timestamp_;
    unsigned interval_;
    unsigned repeats_;
    Type type_;
    unsigned id_;
};

} // namespace EventTimerNS

#endif // EVENT_HH
