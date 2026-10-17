cmake_minimum_required(VERSION 3.10)
project(NavGridApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add headers and sources
set(SOURCES
    src/main.cpp
    src/App.cpp
    src/NavGrid.cpp
    src/GlobalKey.cpp
    src/NavWin.cpp
    src/utils.cpp
)

set(HEADERS
    src/App.hpp
    src/NavGrid.hpp
    src/GlobalKey.hpp
    src/NavWin.hpp
    src/utils.hpp
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# Link necessary WinAPI libraries
target_link_libraries(${PROJECT_NAME} user32 gdi32 msimg32 gdiplus)
