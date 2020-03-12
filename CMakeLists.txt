cmake_minimum_required(VERSION 3.5)

project(2d-engine CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)
add_executable(2d-engine
    player.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    textures.qrc
  )

target_link_libraries(2d-engine PRIVATE Qt5::Core Qt5::Gui)
