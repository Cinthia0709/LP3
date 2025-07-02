cmake_minimum_required(VERSION 3.14)

project(ToursApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)


set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

qt_add_executable(ToursApp
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)


target_link_libraries(ToursApp
    PRIVATE Qt6::Widgets Qt6::Sql
)


qt_finalize_executable(ToursApp)
