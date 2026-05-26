cmake_minimum_required(VERSION 3.25) # Hinweis: VERSION 4.2 gibt es (noch) nicht, 3.25+ ist sicherer
project(untitled)

set(CMAKE_CXX_STANDARD 23)

# 1. Das Executable definieren (Name: untitled)
add_executable(untitled Chip8.cpp Chip8.h)

# 2. SFML suchen
find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)

# 3. SFML mit dem korrekten Target (untitled) verknüpfen
target_link_libraries(untitled PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)