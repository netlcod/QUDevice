cmake_minimum_required(VERSION 3.14)

set(PROJECT qutcpserver)
project(${PROJECT} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_POSTFIX d)

add_library(${PROJECT})

find_package(QT NAMES Qt6 Qt5)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Widgets
    Network
)

set(QUDEVICE_SOURCES
    ${QUDEVICE_DIR}/src/qudevice.cpp
    ${QUDEVICE_DIR}/src/quconfiguration.cpp
    ${QUDEVICE_DIR}/src/quwidget.cpp
)

set(QUDEVICE_INCLUDE
    ${QUDEVICE_DIR}/include/qudevice_export.h
    ${QUDEVICE_DIR}/include/qudevice.h
    ${QUDEVICE_DIR}/include/quconfiguration.h
    ${QUDEVICE_DIR}/include/quwidget.h
)

set(TCPSERVER_SOURCES
    src/qutcpserver.cpp
    src/qutcpserverconfiguration.cpp
    src/qutcpserverwidget.cpp
)

set(TCPSERVER_INCLUDE
    include/qutcpserver.h
    include/qutcpserverconfiguration.h
    include/qutcpserverwidget.h
)

include(GenerateExportHeader)
set(export_file_name include/qudevice_shared.h)

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name include/qudevice_static.h)
endif()

generate_export_header(${PROJECT}
    BASE_NAME QUDEVICE
    EXPORT_FILE_NAME ${export_file_name}
)

target_sources(${PROJECT} PRIVATE
    ${QUDEVICE_SOURCES}
    ${QUDEVICE_INCLUDE}
    ${TCPSERVER_SOURCES}
    ${TCPSERVER_INCLUDE}
)

target_compile_definitions(${PROJECT}
    PUBLIC
        $<$<BOOL:${BUILD_SHARED_LIBS}>:QUDEVICE_SHARED>
)

target_include_directories(${PROJECT}
    PUBLIC
        $<BUILD_INTERFACE:${QUDEVICE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(${PROJECT}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
)

set_target_properties(${PROJECT} PROPERTIES
    PUBLIC_HEADER "${public_headers}"
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
)

if(QUDEVICE_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    install(TARGETS ${PROJECT}
        EXPORT ${PROJECT}_export
        RUNTIME COMPONENT ${PROJECT} DESTINATION ${CMAKE_BUILD_TYPE}/bin
        LIBRARY COMPONENT ${PROJECT} DESTINATION ${CMAKE_BUILD_TYPE}/lib
        ARCHIVE COMPONENT ${PROJECT} DESTINATION ${CMAKE_BUILD_TYPE}/lib
    )

    install(FILES ${QUDEVICE_INCLUDE}
        COMPONENT ${PROJECT}
        DESTINATION ${CMAKE_BUILD_TYPE}/include
    )

    install(FILES ${TCPSERVER_INCLUDE}
        COMPONENT ${PROJECT}
        DESTINATION ${CMAKE_BUILD_TYPE}/include/${PROJECT}
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${export_file_name}
        COMPONENT ${PROJECT}
        DESTINATION ${CMAKE_BUILD_TYPE}/include/${PROJECT}
    )
endif()
