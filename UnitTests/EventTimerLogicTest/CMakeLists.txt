project(EventTimerLogicTest)
set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Test REQUIRED)
add_definitions(-std=c++11)

set (SRC_DIR ../../EventTimer/src)
set (INCLUDE_DIR ../../EventTimer/inc)
set (QT_LIBRARIES Qt5::Core Qt5::Sql)
set (QT_QTTEST_LIBRARY Qt5::Test)

set (TEST_HDRS
        ${INCLUDE_DIR}/event.hh
        ${INCLUDE_DIR}/eventtimer.hh
        ${INCLUDE_DIR}/eventhandler.hh
        ${INCLUDE_DIR}/eventtimerbuilder.hh
        ${INCLUDE_DIR}/logger.hh
)

set (TEST_SRCS
        ${SRC_DIR}/databasehandler.cc
        ${SRC_DIR}/event.cc
        ${SRC_DIR}/eventtimerlogic.cc
        ${SRC_DIR}/eventtimerbuilder.cc
)

include_directories(${INCLUDE_DIR})
include_directories(${SRC_DIR})

set (SRC tst_eventtimerlogictest.cc)
set (TEST_LIBRARIES ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES})

ADD_EXECUTABLE( ${PROJECT_NAME} ${TEST_HDRS} ${TEST_SRCS} ${SRC})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${TEST_LIBRARIES} )
ADD_TEST( NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} )
