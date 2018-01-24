
MACRO(use_cxx11)
  IF (CMAKE_VERSION VERSION_LESS "3.1")
    IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      SET (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    ENDIF ()
  ELSE ()
    SET (CMAKE_CXX_STANDARD 11)
  ENDIF ()
ENDMACRO(use_cxx11)

PROJECT(SumpMonitor)
FIND_PACKAGE(Qt5Core)
IF(NOT Qt5Core_FOUND)
    MESSAGE(ERROR "Qt5 libraries not found")
ENDIF(NOT Qt5Core_FOUND)
FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Sql Network)

ENABLE_LANGUAGE(C)
ENABLE_LANGUAGE(CXX)
use_cxx11()
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# Get the current working branch
EXECUTE_PROCESS(
  COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
        )

# Get the latest abbreviated commit hash of the working branch
EXECUTE_PROCESS(
  COMMAND git describe --always --tags
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
        )

ADD_LIBRARY( wiringPi STATIC ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/wiringPi.c	
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/wiringSerial.c 
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/wiringShift.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/piHiPri.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/piThread.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/wiringPiSPI.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/wiringPiI2C.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/softPwm.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/softTone.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp23008.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp23016.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp23017.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp23s08.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp23s17.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/sr595.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/pcf8574.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/pcf8591.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp3002.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp3004.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp4802.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/mcp3422.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/max31855.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/max5322.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/ads1115.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/sn3218.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/bmp180.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/htu21d.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/ds18b20.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/rht03.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/drcSerial.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/drcNet.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/pseudoPins.c
                             ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi/wpiExtensions.c )

TARGET_INCLUDE_DIRECTORIES(wiringPi PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/wiringPi/wiringPi)
TARGET_LINK_LIBRARIES(wiringPi m pthread rt crypt) 
TARGET_COMPILE_DEFINITIONS(wiringPi PRIVATE _GNU_SOURCE)

ADD_EXECUTABLE( sumpmonitor ${CMAKE_SOURCE_DIR}/src/main.cpp
                            ${CMAKE_SOURCE_DIR}/src/notifier.cpp
                            ${CMAKE_SOURCE_DIR}/src/monitor.cpp
                            ${CMAKE_SOURCE_DIR}/src/ultrasonicSensor.cpp
                            ${CMAKE_SOURCE_DIR}/src/floatSensor.cpp
                            ${CMAKE_SOURCE_DIR}/src/etapeSensor.cpp
                            ${CMAKE_SOURCE_DIR}/src/postsqldata.cpp
                            ${CMAKE_SOURCE_DIR}/src/sumpdata.cpp
                            ${CMAKE_SOURCE_DIR}/src/network.cpp ) 

TARGET_LINK_LIBRARIES(sumpmonitor wiringPi Qt5::Core Qt5::Network Qt5::Sql )
TARGET_INCLUDE_DIRECTORIES(sumpmonitor PRIVATE ${CMAKE_SOURCE_DIR}/src)
TARGET_COMPILE_DEFINITIONS(sumpmonitor PRIVATE GIT_VERSION="${GIT_VERSION}")

