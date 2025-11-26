cmake_minimum_required(VERSION 3.28.3)
project(pacman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML (using your path)
set(SFML_DIR "/usr/local/opt/sfml@2/lib/cmake/SFML")
find_package(SFML 2 COMPONENTS system window graphics audio network REQUIRED)

# Add the logic library FIRST
add_subdirectory(logic)

# Add the game executable SECOND
add_subdirectory(app)