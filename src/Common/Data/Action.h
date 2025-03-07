#pragma once

#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <string>
#include <variant>
#include <vector>

class ActionData {
 public:
  struct Wait {
    static constexpr const char* HEADER = "<Wait>";
    float durationSeconds;
  };
  struct WaitInput {
    static constexpr const char* HEADER = "<WaitInput>";
  };
  struct Text {
    static constexpr const char* HEADER = "<Text>";
    std::string text;
  };
  struct TextFont {
    static constexpr const char* HEADER = "<TextFont>";
    std::filesystem::path filename;
  };
  struct TextSound {
    static constexpr const char* HEADER = "<TextSound>";
    std::filesystem::path filename;
  };
  struct TextRate {
    static constexpr const char* HEADER = "<TextRate>";
    float value;
  };
  struct CharName {
    static constexpr const char* HEADER = "<CharName>";
    std::string text;
  };
  struct CharNameFont {
    static constexpr const char* HEADER = "<CharNameFont>";
    std::filesystem::path filename;
  };
  struct CharIcon {
    static constexpr const char* HEADER = "<CharIcon>";
    std::filesystem::path filename;
  };
  struct Image {
    static constexpr const char* HEADER = "<Image>";
    std::filesystem::path filename;
  };
  struct Sound {
    static constexpr const char* HEADER = "<Sound>";
    std::filesystem::path filename;
    bool looping = false;
    float offsetSeconds = 0.f;
    float volume = 100.f;
  };
  struct Music {
    static constexpr const char* HEADER = "<Music>";
    std::filesystem::path filename;
    bool looping = false;
    float offsetSeconds = 0.f;
    float volume = 100.f;
  };
  struct MusicPlay {
    static constexpr const char* HEADER = "<MusicPlay>";
  };
  struct MusicPause {
    static constexpr const char* HEADER = "<MusicPause>";
  };
  struct MusicStop {
    static constexpr const char* HEADER = "<MusicStop>";
  };
  struct Animation {
    static constexpr const char* HEADER = "<Animation>";
    int entity;  // entityId_t?
    std::vector<int> frames;
    float frameRate;
    int startFrameIndex;
  };
  struct Position {
    static constexpr const char* HEADER = "<Position>";
    int entity;  // entityId_t?
    sf::Vector2i position;
  };
  struct Move {
    static constexpr const char* HEADER = "<Move>";
    int entity;  // entityId_t?
    sf::Vector2i direction;
    float durationSeconds;
  };
  struct PositionCamera {
    static constexpr const char* HEADER = "<PositionCamera>";
    sf::Vector2f position;
  };
  struct MoveCamera {
    static constexpr const char* HEADER = "<MoveCamera>";
    sf::Vector2f direction;
    float durationSeconds;
  };
  struct FadeIn {
    static constexpr const char* HEADER = "<FadeIn>";
    float durationSeconds;
  };
  struct FadeOut {
    static constexpr const char* HEADER = "<FadeOut>";
    float durationSeconds;
  };
  struct Level {
    static constexpr const char* HEADER = "<Level>";
    std::filesystem::path filename;
    int playerSpot = 0;
  };
  struct Tile {
    static constexpr const char* HEADER = "<Tile>";
    sf::Vector2i position;
    int tile;
  };
  struct Collider {
    static constexpr const char* HEADER = "<Collider>";
    sf::Vector2i position;
    bool value;
  };
  struct Action {
    static constexpr const char* HEADER = "<Action>";
    // TODO(des): set action action
  };
  struct State {
    static constexpr const char* HEADER = "<State>";
    // TODO(des): set state action
  };
  struct Multiple {
    static constexpr const char* HEADER = "<Multiple>";
    std::vector<ActionData> actions;
  };

  template <typename ActionType>
  ActionData(const ActionType& action) : data(action) {}

  std::variant<Wait, WaitInput, Text, TextFont, TextSound, TextRate, CharName,
               CharNameFont, CharIcon, Image, Sound, Music, MusicPlay,
               MusicPause, MusicStop, Animation, Position, Move, PositionCamera,
               MoveCamera, FadeIn, FadeOut, Level, Tile, Collider, Action,
               State, Multiple>
      data;

  static ActionData load(std::istream& in);
  void save(std::ostream& out);
};
