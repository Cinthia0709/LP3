cmake_minimum_required(VERSION 3.16)

project(CRUDnodos LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(CRUDnodos
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(CRUDnodos PRIVATE Qt6::Widgets Qt6::Sql)
