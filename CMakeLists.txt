cmake_minimum_required(VERSION 3.28.3)
project(pacman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# AUTOMATISCHE DETECTIE (Hints voor Mac gebruikers)
# ---------------------------------------------------------------------------
if (APPLE)
    # Als we op een Mac zitten, voeg dan de bekende Homebrew paden toe aan de zoeklijst.
    # Dit is geen harde verplichting, maar een "hint" voor CMake.

    # Pad voor Intel Macs (zoals de jouwe waarschijnlijk)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/sfml@2")

    # Pad voor Apple Silicon Macs (M1/M2/M3) - voor de zekerheid
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")
endif()

# ---------------------------------------------------------------------------
# FIND PACKAGE (Werkt nu overal)
# ---------------------------------------------------------------------------
# CMake zoekt nu in systeem mappen (Linux) EN in de hints hierboven (Mac)
find_package(SFML 2 COMPONENTS system window graphics audio network REQUIRED)

# ---------------------------------------------------------------------------
# PROJECT OPBOUW
# ---------------------------------------------------------------------------
add_subdirectory(logic)
add_subdirectory(app)