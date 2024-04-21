cmake_minimum_required(VERSION 3.24)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 11)

add_executable(Minesweeper main.cpp
        Tile.cpp
        Tile.h
        Board.cpp
        Board.h
        TrayGui.cpp
        TrayGui.h
        file_read_exception.cpp
        file_read_exception.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(c:/SFML/include/SFML)
target_link_libraries(Minesweeper sfml-system sfml-window sfml-graphics sfml-audio)