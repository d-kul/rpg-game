cmake_minimum_required(VERSION 3.28)
project(rpg_game 
  LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.0
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)

FetchContent_Declare(
  ImGui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.91.8
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)

FetchContent_Declare(
  ImGui-SFML
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml
  GIT_TAG v3.0
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)

# SFML
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(SFML)

# Dear ImGui
FetchContent_MakeAvailable(ImGui)

# ImGui-SFML
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
FetchContent_MakeAvailable(ImGui-SFML)

add_executable(game 
  src/Core/Signal.h
  src/Core/ConnectionGuard.cpp
  src/Core/Config.h
  src/Core/Config.cpp
  src/Core/Logger.h
  src/Core/Logger.cpp
  src/Core/utility.h
  src/Common/Data/Action.h
  src/Common/Data/Action.cpp
  src/Common/Data/Entity.h
  src/Common/Data/Entity.cpp
  src/Common/Data/Level.h
  src/Common/Data/Level.cpp
  src/Manager/Event.h
  src/Manager/Audio.h
  src/Manager/Audio.cpp
  src/Manager/Resource.h
  src/Manager/UI.h
  src/Manager/UI.cpp
  src/Manager/Interactible.h
  src/Manager/Interactible.cpp
  src/Manager/Collider.h
  src/Manager/Collider.cpp
  src/UI/Element.h
  src/UI/Element.cpp
  src/UI/Frame.h
  src/UI/Frame.cpp
  src/UI/Text.h
  src/UI/Text.cpp
  src/UI/Button.h
  src/UI/Button.cpp
  src/Action.h
  src/Action.cpp
  src/Action/FadeInAction.h
  src/Action/WaitAction.h
  src/Action/TileAction.h
  src/Action/MultipleAction.h
  src/Action/CharNameAction.h
  src/Action/TextAction.h
  src/Action/TextSoundAction.h
  src/Action/MoveCameraAction.h
  src/Action/MoveAction.h
  src/Action/MusicPlayAction.h
  src/Action/ColliderAction.h
  src/Action/PositionCameraAction.h
  src/Action/MusicPauseAction.h
  src/Action/MusicAction.h
  src/Action/TextRateAction.h
  src/Action/CharIconAction.h
  src/Action/SoundAction.h
  src/Action/SoundAction.cpp
  src/Action/AnimationAction.h
  src/Action/MusicStopAction.h
  src/Action/WaitInputAction.h
  src/Action/FadeOutAction.h
  src/Action/PositionAction.h
  src/Action/CharNameFontAction.h
  src/Action/ImageAction.h
  src/Action/LevelAction.h
  src/Resource/TileSet.h
  src/Resource/TileSet.cpp
  src/Entity.h
  src/Entity/Movable.h
  src/Entity/Movable.cpp
  src/Entity/Animated.h
  src/Entity/Animated.cpp
  src/Entity/Interactible.h
  src/Entity/Interactible.cpp
  src/Entity/Actor.h
  src/Entity/Player.h
  src/Entity/Player.cpp
  src/Entity/Character.h
  src/Entity/Prop.h
  src/Entity/Collider.h
  src/Entity/Collider.cpp
  src/Entity/RectCollider.h
  src/Entity/RectCollider.cpp
  src/Entity/TileMapCollider.h
  src/Entity/TileMapCollider.cpp
  src/Entity/Background.h
  src/Entity/Background.cpp
  src/Entity/TileMap.h
  src/Entity/TileMap.cpp
  src/State.h
  src/State.cpp
  src/State/GameState.h
  src/State/GameState.cpp
  src/State/MainMenuState.h
  src/State/MainMenuState.cpp
  src/Level.h
  src/Level.cpp
  src/Game.h
  src/Game.cpp
  src/Game_init.cpp
  src/main.cpp)
target_include_directories(game PRIVATE src)
target_compile_features(game PRIVATE cxx_std_17)
target_link_libraries(game PRIVATE SFML::Audio SFML::Graphics)

add_executable(editor 
  src/Core/Logger.h
  src/Core/Logger.cpp
  src/Common/Data/Action.h
  src/Common/Data/Action.cpp
  src/Common/Data/Entity.h
  src/Common/Data/Entity.cpp
  src/Common/Data/Level.h
  src/Common/Data/Level.cpp
  src/Entity/Background.h
  src/Entity/Background.cpp
  src/Resource/TileSet.h
  src/Resource/TileSet.cpp
  src/Editor/Grid.h
  src/Editor/Grid.cpp
  src/Editor/Editor.h
  src/Editor/Editor.cpp
  src/Editor/main.cpp)
target_include_directories(editor PRIVATE src)
target_compile_features(editor PRIVATE cxx_std_17)
target_link_libraries(editor PRIVATE ImGui-SFML::ImGui-SFML)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
install(TARGETS game RUNTIME DESTINATION game)
install(DIRECTORY config resources DESTINATION game)
install(FILES LICENSE.txt README.txt DESTINATION game)
