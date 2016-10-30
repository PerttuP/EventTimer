
project(EventTimer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
add_definitions(-std=c++11)

set (INCLUDE_DIR "inc")
set (SRC_DIR "src")

set (INST_HDRS 
        ${INCLUDE_DIR}/event.hh
        ${INCLUDE_DIR}/eventhandler.hh
        ${INCLUDE_DIR}/eventtimer.hh
        ${INCLUDE_DIR}/eventtimerbuilder.hh
        ${INCLUDE_DIR}/logger.hh
        ${INCLUDE_DIR}/EventTimerConfig.h.in
)
	
set (SRC
        ${SRC_DIR}/databasehandler.cc
        ${SRC_DIR}/event.cc
        ${SRC_DIR}/eventtimerbuilder.cc
        ${SRC_DIR}/eventtimerlogic.cc
)

configure_file( ${PROJECT_SOURCE_DIR}/${INCLUDE_DIR}/${PROJECT_NAME}Config.h.in
                ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h )
	
include_directories(${INCLUDE_DIR} ${PROJECT_BINARY_DIR})

add_library(${PROJECT_NAME} SHARED ${INST_HDRS} ${SRC} )

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Sql)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
)

install(FILES ${INST_HDRS} DESTINATION include)
