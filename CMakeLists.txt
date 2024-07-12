cmake_minimum_required(VERSION 3.14)

set(PROJECT QUDevice)
project(${PROJECT} VERSION 1.0.0 LANGUAGES CXX)

set(QUDEVICE_DIR ${CMAKE_SOURCE_DIR} CACHE STRING ${CMAKE_SOURCE_DIR})

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(QUDEVICE_INSTALL "Install available devices" OFF)
