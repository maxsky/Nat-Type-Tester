cmake_minimum_required(VERSION 3.20)
project(NatTypeTester)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(MODULES REQUIRED gtk+-3.0)

ADD_DEFINITIONS(${MODULES_CFLAGS_OTHER})
INCLUDE_DIRECTORIES(${MODULES_INCLUDE_DIRS})
LINK_DIRECTORIES(${MODULES_LIBRARY_DIRS})

add_executable(NatTypeTester main.cpp
        source/message/msgstruct.h
        source/message/stunmsg.cpp source/message/stunmsg.h
        source/message/endecode.cpp source/message/endecode.h
        source/component.cpp source/component.h
        source/layout.cpp source/layout.h
        source/gsocket.cpp source/gsocket.h)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GTK3_LIBRARIES})
