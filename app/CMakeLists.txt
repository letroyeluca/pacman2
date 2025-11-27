# Create the final executable
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(pacman_game
        src/Game.cpp
        src/main.cpp
        src/states/MenuState.cpp
        src/states/GameState.cpp
        src/states/StateManager.cpp
        src/Camera.cpp
)

target_include_directories(pacman_game PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(pacman_game PRIVATE
        ../logic/include/logic
)
target_link_libraries(pacman_game PRIVATE
        pacman_logic
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

add_custom_command(TARGET pacman_game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:pacman_game>/assets
)