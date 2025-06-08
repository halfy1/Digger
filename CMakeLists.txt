cmake_minimum_required(VERSION 3.5.0)
project(Digger VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(libs/Data-Structures)

find_package(SDL2 REQUIRED)

# Явно ищем SDL2_ttf (вместо find_package)
find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h PATH_SUFFIXES SDL2)
find_library(SDL2_TTF_LIBRARY SDL2_ttf)

if (NOT SDL2_TTF_INCLUDE_DIR OR NOT SDL2_TTF_LIBRARY)
    message(FATAL_ERROR "SDL2_ttf not found!")
endif()

set (sources
    main.cpp
    src/Tile.cpp
    src/Board.cpp
    src/Renderer.cpp
    src/Game.cpp
    src/Fireball.cpp
    src/Collectible/Collectible.cpp
    src/Collectible/Emerald.cpp
    src/Collectible/Gold.cpp
    src/Entity/Entity.cpp
    src/Entity/Enemy.cpp
    src/Entity/Player.cpp
    src/SimpleMenu.cpp
)

add_executable(${PROJECT_NAME} ${sources})

list(APPEND includePath
    "include"
    "include/Collectible"
    "include/Entity"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${includePath}
        ${CMAKE_SOURCE_DIR}/libs/Data-Structures/include
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Data-Structures
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARY}
)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)