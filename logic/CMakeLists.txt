
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(pacman_logic STATIC
        src/World.cpp

    )
target_include_directories(pacman_logic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(pacman_logic PUBLIC
        ../app/include
)