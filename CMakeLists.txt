cmake_minimum_required(VERSION 3.16)

project(OvercookedQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(OvercookedQt
    main.cpp
    Theme.h
    MainWindow.h
    MainWindow.cpp
    GameScene.h
    GameScene.cpp
    KitchenItem.h
    KitchenItem.cpp
    PlayerItem.h
    PlayerItem.cpp
    MenuOverlayItem.h
    MenuOverlayItem.cpp
    DialogOverlayItem.h
    DialogOverlayItem.cpp
    TutorialOverlayItem.h
    TutorialOverlayItem.cpp
    HudOverlayItem.h
    HudOverlayItem.cpp
    ResultOverlayItem.h
    ResultOverlayItem.cpp
)

target_link_libraries(OvercookedQt PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
