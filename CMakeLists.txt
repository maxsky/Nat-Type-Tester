cmake_minimum_required(VERSION 3.20)
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
        source/message/stunmsg.cpp source/message/stunmsg.h
        source/message/endecode.cpp source/message/endecode.h
        source/component.cpp source/component.h
        source/layout.cpp source/layout.h
        source/gsocket.cpp source/gsocket.h)

SET(LINK_PARAMS ${PROJECT_NAME} ${MODULES_LIBRARIES})

IF (WIN32)
    TARGET_LINK_LIBRARIES(${LINK_PARAMS} -lwsock32)
ELSE ()
    TARGET_LINK_LIBRARIES(${LINK_PARAMS})
ENDIF ()
