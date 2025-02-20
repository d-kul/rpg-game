cmake_minimum_required(VERSION 3.28)
project(rpg_game 
  LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main 
  src/Core/Signal.h
  src/Core/ConnectionGuard.cpp
  src/Core/Config.h
  src/Core/Config.cpp
  src/Core/Logger.h
  src/Core/Logger.cpp
  src/Core/utility.h
  src/Manager/Event.h
  src/Manager/Audio.h
  src/Manager/Audio.cpp
  src/Manager/Resource.h
  src/Manager/Interactible.h
  src/Manager/Interactible.cpp
  src/UI/Button.h
  src/UI/Button.cpp
  src/Resource/TileSet.h
  src/Resource/TileSet.cpp
  src/AnimatedSprite.h
  src/AnimatedSprite.cpp
  src/Entity.h
  src/Entity/Background.h
  src/Entity/Background.cpp
  src/Entity/TileMap.h
  src/Entity/TileMap.cpp
  src/Entity/Player.h
  src/Entity/Player.cpp
  src/Entity/Interactible.h
  src/Entity/Interactible.cpp
  src/State.h
  src/State.cpp
  src/State/GameState.h
  src/State/GameState.cpp
  src/State/MainMenuState.h
  src/State/MainMenuState.cpp
  src/Game.h
  src/Game.cpp
  src/Game_init.cpp
  src/main.cpp)
target_include_directories(main PRIVATE src)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Audio SFML::Graphics)
