
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

project(EventTimerLib)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

add_subdirectory(EventTimer)
add_subdirectory(UnitTests)
#add_subdirectory(EventTimerExample)
