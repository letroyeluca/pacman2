
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(pacman_logic STATIC
        src/World.cpp
        src/utils/Random.cpp
        src/models/EntityModel.cpp
        src/models/WallModel.cpp
        src/models/CoinModel.cpp
        src/models/PacManModel.cpp
        src/utils/MapLoader.cpp
        src/utils/txtMapLoader.cpp
        include/logic/Event.h
        src/utils/StopWatch.cpp

)
target_include_directories(pacman_logic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(pacman_logic PUBLIC
        ../app/include
)