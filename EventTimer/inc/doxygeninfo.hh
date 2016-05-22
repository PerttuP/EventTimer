#ifndef DOXYGENINFO_HH
#define DOXYGENINFO_HH

/** @file
 * @brief Front page of EventTimer doxygen-documentation.
 * @author Perttu Paarlahti 2016.
 */

/** @mainpage The EventTimer component documentation.
 *
 * @section General
 *
 * The EventTimer component is a generalized version of the SignalGenerator application used in the Auxilo2 smart home project
 * (http://github.com/coder4803/auxilo2). SignalGenerator was a stand alone application that communicated with other Auxilo2 applications
 * through common MessageBroker application. Being a complete application restricts its reuability in some other context. EventTimer is
 * designed to be a reusable component, that is not dependent on Auxilo2 components or architecture, and therefore can be reused in totally
 * different context. The SignalGenerator application may be later refactored to use this component to ease maintenance and deploy extra features
 * added into EventTimer.
 *
 * This software is published under MIT-licence. See the LICENCE-file in the git repository root for detailed conditions.
 *
 *
 * @section Features
 *
 * General purpose: The EventTimer component allows user to store 'events' into a persistent database, and be notified, when these events occur.
 * Event is an object having unique id, non-unique name, timestamp, type, interval and repeat times. See EventTimerNS::Event for detailed description.
 * The EventTimer interface allows the user to:
 *
 * 1) Add events in the persistent database (see EventTimerNS::EventTimer::addEvent).
 *
 * 2) Remove individual events, all dynamic events or all events from the database (see see EventTimerNS::EventTimer::removeEvent,
 * EventTimerNS::EventTimer::clearDynamic and EventTimerNS::EventTimer::clearAll).
 *
 * 3) Query status of individual events (see EventTimerNS::EventTimer::getEvent).
 *
 * 4) Query list of next occuring events (see EventTimerNS::EventTimer::nextEvents).
 *
 * 5) Set or change custom handler for reacting to occuring events (see EventTimerNS::EventTimer::setEventHandler and EventTimerNS::EventHandler).
 *
 * 6) Enable or disable log messages from the EventTimer with a custom log handler (see EventTimerNS::EventTimer::setLogger and EventTimerNS::Logger).
 *
 * 7) Start and stop the EventTimer at any time (see EventTimerNS::EventTimer::start and EventTimerNS::EventTimer::stop).
 *
 *
 *
 * @section Contents of this document
 *
 * This document describes public interfaces and classes in the EventTimer component. Public class documentation consists of description of
 * all available methods, and how to use them appropriately (pre- and post conditions). Interface implementations or other internal details are
 * not included, even though these classes are documented in the code in a similiar way using doxygen. Test projects and examples are not
 * included eather.
 *
 * Start exploring the document using the top bar menu.
 */

#endif // DOXYGENINFO_HH

