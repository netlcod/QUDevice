cmake_minimum_required(VERSION 3.14)

set(PROJECT QUDevice)
project(${PROJECT} VERSION 1.0.0 LANGUAGES CXX)

set(QUDEVICE_DIR ${CMAKE_SOURCE_DIR} CACHE STRING ${CMAKE_SOURCE_DIR})

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(QUDEVICE_INSTALL "Install available devices" OFF)

option(BUILD_ALL_QUDEVICES "Build all available devices" OFF)
option(BUILD_QUASYNCSERIALPORT "BUILD_QUASYNCSERIALPORT" OFF)
option(BUILD_QUBLOCKINGSERIALPORT "BUILD_QUBLOCKINGSERIALPORT" OFF)
option(BUILD_QUASYNCUDP "BUILD_QUASYNCUDP" OFF)
option(BUILD_QUBLOCKINGUDP "BUILD_QUBLOCKINGUDP" OFF)
option(BUILD_QUMODBUSMASTER "BUILD_QUMODBUSMASTER" OFF)
option(BUILD_QUTCPCLIENT "BUILD_QUTCPCLIENT" OFF)
option(BUILD_QUTCPSERVER "BUILD_QUTCPSERVER" OFF)

if (BUILD_ALL_QUDEVICES OR BUILD_QUASYNCSERIALPORT)
    add_subdirectory(devices/quasyncserialport)
endif()

if (BUILD_ALL_QUDEVICES OR BUILD_QUBLOCKINGSERIALPORT)
    add_subdirectory(devices/qublockingserialport)
endif()

if (BUILD_ALL_QUDEVICES OR BUILD_QUASYNCUDP)
    add_subdirectory(devices/quasyncudp)
endif()

if (BUILD_ALL_QUDEVICES OR BUILD_QUBLOCKINGUDP)
    add_subdirectory(devices/qublockingudp)
endif()

if (BUILD_ALL_QUDEVICES OR BUILD_QUMODBUSMASTER)
    add_subdirectory(devices/qumodbusmaster)
endif()

if (BUILD_ALL_QUDEVICES OR BUILD_QUTCPCLIENT)
    add_subdirectory(devices/qutcpclient)
endif()

if (BUILD_ALL_QUDEVICES OR BUILD_QUTCPSERVER)
    add_subdirectory(devices/qutcpserver)
endif()
