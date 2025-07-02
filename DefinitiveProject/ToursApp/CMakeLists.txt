cmake_minimum_required(VERSION 3.5)

project(ToursApp LANGUAGES CXX)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 REQUIRED COMPONENTS Widgets)


add_executable(ToursApp
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(ToursApp PRIVATE Qt5::Widgets)
