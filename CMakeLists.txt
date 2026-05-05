cmake_minimum_required(VERSION 3.16)

project(OvercookedQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(OvercookedQt
    main.cpp
    gameconstants.h
    gamewindow.h
    gamewindow.cpp
    gameview.h
    gameview.cpp
    gamescene.h
    gamescene.cpp
    player.h
    player.cpp
    station.h
    station.cpp
    tileitem.h
    tileitem.cpp
    huditem.h
    huditem.cpp
)

target_link_libraries(OvercookedQt PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
