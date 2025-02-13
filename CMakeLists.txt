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

FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.14.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(EnTT)

add_executable(main 
  src/Config.cpp
  src/Config.h
  src/State.cpp
  src/State.h
  src/State/GameState.cpp
  src/State/GameState.h
  src/State/MainMenuState.cpp
  src/State/MainMenuState.h
  src/Component/MovementComponent.h
  src/Component/PlayerComponent.h
  src/System/RenderSystem.cpp
  src/System/RenderSystem.h
  src/System/SoundSystem.cpp
  src/System/SoundSystem.h
  src/System/MovementSystem.cpp
  src/System/MovementSystem.h
  src/System/EventHandlerSystem.cpp
  src/System/EventHandlerSystem.h
  src/System/PlayerSystem.cpp
  src/System/PlayerSystem.h
  src/Button.cpp
  src/Button.h
  src/Game.cpp
  src/Game_init.cpp
  src/Game.h
  src/utility.h
  src/main.cpp)
target_include_directories(main PRIVATE src)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Audio SFML::Graphics EnTT::EnTT)
