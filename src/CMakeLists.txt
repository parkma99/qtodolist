cmake_minimum_required(VERSION 3.16)

project(Qtodolist VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
qt_standard_project_setup()

qt_add_executable(Qtodolist
    qitem.cpp
    qitem.ui
    qitem.h
    qitemlist.cpp
    qtodolist.cpp
    qtodolist.ui
    qtodolist.h
    qtodolist.qrc
    sidebarwidget.cpp
    sidebarwidget.ui
    sidebarwidget.h
    sqldata.h
    sqldata.cpp
    timedialog.cpp
    timedialog.ui
    timedialog.h
    main.cpp
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

qt_add_resources(Qtodolist qtodolist.qrc)
target_link_libraries(Qtodolist PRIVATE Qt6::Widgets Qt6::Sql)

set_target_properties(Qtodolist PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
