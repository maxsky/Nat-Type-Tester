cmake_minimum_required(VERSION 3.20)
project(NetTypeTester)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

add_executable(NetTypeTester main.cpp source/interface.cpp source/interface.h source/type_test.cpp source/type_test.h)

TARGET_LINK_LIBRARIES(NetTypeTester ${GTK3_LIBRARIES})
