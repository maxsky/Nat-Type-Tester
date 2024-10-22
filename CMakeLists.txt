cmake_minimum_required(VERSION 3.5)
project(NatTypeTester)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(MODULES REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${MODULES_INCLUDE_DIRS})
LINK_DIRECTORIES(${MODULES_LIBRARY_DIRS})
ADD_DEFINITIONS(${MODULES_CFLAGS_OTHER})

add_executable(NatTypeTester main.cpp
        source/message/msgstruct.h
        source/window/component.cpp source/window/component.h
        source/window/layout.cpp source/window/layout.h
        source/socket/client.cpp source/socket/client.h source/socket/socket.cpp source/socket/socket.h)

SET(LINK_PARAMS ${PROJECT_NAME} ${MODULES_LIBRARIES})

IF (WIN32)
    TARGET_LINK_LIBRARIES(${LINK_PARAMS} -mwindows -lwsock32)
ELSE ()
    TARGET_LINK_LIBRARIES(${LINK_PARAMS})
ENDIF ()
