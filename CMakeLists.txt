cmake_minimum_required(VERSION 3.20)
project(NatTypeTester)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

add_executable(NatTypeTester main.cpp source/component.cpp source/component.h source/layout.cpp source/layout.h)

TARGET_LINK_LIBRARIES(NatTypeTester ${GTK3_LIBRARIES})
