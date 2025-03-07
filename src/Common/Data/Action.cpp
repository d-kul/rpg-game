#include "Action.h"

#include <stdexcept>

#include "Core/Logger.h"
#include "Core/utility.h"

// NOTE(des): template black magic fuckery ahead, might not compile everywhere
// due to compiler bugs

namespace {

template <typename T>
T load(std::istream& in);

template <typename... Ts>
struct Load {
  static ActionData load(std::string& line, std::istream& in) {
    ERROR("invalid action header");
    throw std::runtime_error("invalid action header");
  }
};

template <typename T, typename... Ts>
struct Load<T, Ts...> {
  static ActionData load(std::string& line, std::istream& in) {
    if (line != T::HEADER) return Load<Ts...>::load(line, in);
    return ActionData(::load<T>(in));
  }
};

template <typename T>
struct LoadImpl;

template <typename... Ts>
struct LoadImpl<std::variant<Ts...>> {
  static ActionData load(std::istream& in) {
    std::string line;
    std::getline(in, line);
    return Load<Ts...>::load(line, in);
  }
};

template <typename T>
void save(T& t, std::ostream& out);

template <typename T>
void saveImpl(T& t, std::ostream& out) {
  out << T::HEADER << '\n';
  save<T>(t, out);
}

// NOTE(des): i don't know if this is really the most convenient solution...

// Wait
template <>
ActionData::Wait load(std::istream& in) {
  return {readOne<float>(in)};
}
template <>
void save(ActionData::Wait& wait, std::ostream& out) {
  out << wait.durationSeconds << '\n';
}

// WaitInput
template <>
ActionData::WaitInput load(std::istream& in) {
  return {};
}
template <>
void save(ActionData::WaitInput& waitInput, std::ostream& out) {}

// Text
template <>
ActionData::Text load(std::istream& in) {
  ActionData::Text text;
  in >> std::quoted(text.text) >> std::ws;
  return text;
}
template <>
void save(ActionData::Text& text, std::ostream& out) {
  out << std::quoted(text.text) << '\n';
}

// TextFont
template <>
ActionData::TextFont load(std::istream& in) {
  return {readOne<std::filesystem::path>(in)};
}
template <>
void save(ActionData::TextFont& textFont, std::ostream& out) {
  out << textFont.filename << '\n';
}

// TextSound
template <>
ActionData::TextSound load(std::istream& in) {
  return {readOne<std::filesystem::path>(in)};
}
template <>
void save(ActionData::TextSound& textSound, std::ostream& out) {
  out << textSound.filename << '\n';
}

// TextRate
template <>
ActionData::TextRate load(std::istream& in) {
  return {readOne<float>(in)};
}
template <>
void save(ActionData::TextRate& textRate, std::ostream& out) {
  out << textRate.value << '\n';
}

// CharName
template <>
ActionData::CharName load(std::istream& in) {
  ActionData::CharName name;
  in >> std::quoted(name.text) >> std::ws;
  return name;
}
template <>
void save(ActionData::CharName& charName, std::ostream& out) {
  out << std::quoted(charName.text) << '\n';
}

// CharNameFont
template <>
ActionData::CharNameFont load(std::istream& in) {
  return {readOne<std::filesystem::path>(in)};
}
template <>
void save(ActionData::CharNameFont& charNameFont, std::ostream& out) {
  out << charNameFont.filename << '\n';
}

// CharIcon
template <>
ActionData::CharIcon load(std::istream& in) {
  return {readOne<std::filesystem::path>(in)};
}
template <>
void save(ActionData::CharIcon& charIcon, std::ostream& out) {
  out << charIcon.filename << '\n';
}

// Image
template <>
ActionData::Image load(std::istream& in) {
  return {readOne<std::filesystem::path>(in)};
}
template <>
void save(ActionData::Image& image, std::ostream& out) {
  out << image.filename << '\n';
}

// Sound
template <>
ActionData::Sound load(std::istream& in) {
  ActionData::Sound sound;
  in >> sound.filename >> sound.looping >> sound.offsetSeconds >>
      sound.volume >> std::ws;
  return sound;
}
template <>
void save(ActionData::Sound& sound, std::ostream& out) {
  out << sound.filename << ' ' << sound.looping << ' ' << sound.offsetSeconds
      << ' ' << sound.volume << '\n';
}

// Music
template <>
ActionData::Music load(std::istream& in) {
  ActionData::Music music;
  in >> music.filename >> music.looping >> music.offsetSeconds >>
      music.volume >> std::ws;
  return music;
}
template <>
void save(ActionData::Music& music, std::ostream& out) {
  out << music.filename << ' ' << music.looping << ' ' << music.offsetSeconds
      << ' ' << music.volume << '\n';
}

// MusicPlay
template <>
ActionData::MusicPlay load(std::istream& in) {
  return {};
}
template <>
void save(ActionData::MusicPlay& musicPlay, std::ostream& out) {}

// MusicPause
template <>
ActionData::MusicPause load(std::istream& in) {
  return {};
}
template <>
void save(ActionData::MusicPause& musicPause, std::ostream& out) {}

// MusicStop
template <>
ActionData::MusicStop load(std::istream& in) {
  return {};
}
template <>
void save(ActionData::MusicStop& musicStop, std::ostream& out) {}

// Animation
template <>
ActionData::Animation load(std::istream& in) {
  ActionData::Animation animation;
  std::size_t frames_size;
  in >> animation.entity >> animation.frameRate >> animation.startFrameIndex >>
      frames_size;
  animation.frames.resize(frames_size);
  for (int& frame : animation.frames) {
    in >> frame;
  }
  in >> std::ws;
  return animation;
}
template <>
void save(ActionData::Animation& animation, std::ostream& out) {
  out << animation.entity << ' ' << animation.frameRate << ' '
      << animation.startFrameIndex << ' ' << animation.frames.size() << '\n';
  bool first = true;
  for (int& frame : animation.frames) {
    if (!first) {
      out << ' ';
    } else {
      first = false;
    }
    out << frame;
  }
  out << '\n';
}

// Position
template <>
ActionData::Position load(std::istream& in) {
  ActionData::Position position;
  in >> position.entity >> position.position.x >> position.position.y >>
      std::ws;
  return position;
}
template <>
void save(ActionData::Position& position, std::ostream& out) {
  out << position.entity << ' ' << position.position.x << ' '
      << position.position.y << '\n';
}

// Move
template <>
ActionData::Move load(std::istream& in) {
  ActionData::Move move;
  in >> move.entity >> move.direction.x >> move.direction.y >>
      move.durationSeconds >> std::ws;
  return move;
}
template <>
void save(ActionData::Move& move, std::ostream& out) {
  out << move.entity << ' ' << move.direction.x << ' ' << move.direction.y
      << ' ' << move.durationSeconds << '\n';
}

// PositionCamera
template <>
ActionData::PositionCamera load(std::istream& in) {
  ActionData::PositionCamera positionCamera;
  in >> positionCamera.position.x >> positionCamera.position.y >> std::ws;
  return positionCamera;
}
template <>
void save(ActionData::PositionCamera& positionCamera, std::ostream& out) {
  out << positionCamera.position.x << ' ' << positionCamera.position.y << '\n';
}

// MoveCamera
template <>
ActionData::MoveCamera load(std::istream& in) {
  ActionData::MoveCamera moveCamera;
  in >> moveCamera.direction.x >> moveCamera.direction.y >>
      moveCamera.durationSeconds >> std::ws;
  return moveCamera;
}
template <>
void save(ActionData::MoveCamera& moveCamera, std::ostream& out) {
  out << moveCamera.direction.x << ' ' << moveCamera.direction.y << ' '
      << moveCamera.durationSeconds << '\n';
}

// FadeIn
template <>
ActionData::FadeIn load(std::istream& in) {
  return {readOne<float>(in)};
}
template <>
void save(ActionData::FadeIn& fadeIn, std::ostream& out) {
  out << fadeIn.durationSeconds << '\n';
}

// FadeOut
template <>
ActionData::FadeOut load(std::istream& in) {
  return {readOne<float>(in)};
}
template <>
void save(ActionData::FadeOut& fadeOut, std::ostream& out) {
  out << fadeOut.durationSeconds << '\n';
}

// Level
template <>
ActionData::Level load(std::istream& in) {
  ActionData::Level level;
  in >> level.filename >> level.playerSpot >> std::ws;
  return level;
}
template <>
void save(ActionData::Level& level, std::ostream& out) {
  out << level.filename << ' ' << level.playerSpot << '\n';
}

// Tile
template <>
ActionData::Tile load(std::istream& in) {
  ActionData::Tile tile;
  in >> tile.position.x >> tile.position.y >> tile.tile >> std::ws;
  return tile;
}
template <>
void save(ActionData::Tile& tile, std::ostream& out) {
  out << tile.position.x << ' ' << tile.position.y << ' ' << tile.tile << '\n';
}

// Collider
template <>
ActionData::Collider load(std::istream& in) {
  ActionData::Collider collider;
  in >> collider.position.x >> collider.position.y >> collider.value >> std::ws;
  return collider;
}
template <>
void save(ActionData::Collider& collider, std::ostream& out) {
  out << collider.position.x << ' ' << collider.position.y << ' '
      << collider.value << '\n';
}

// Action
template <>
ActionData::Action load(std::istream& in) {
  ActionData::Action action;
  in >> action.holder >> action.action >> std::ws;
  return action;
}
template <>
void save(ActionData::Action& action, std::ostream& out) {
  out << action.holder << ' ' << action.action << '\n';
}

// State
template <>
ActionData::State load(std::istream& in) {
  return {};  // TODO(des): load state action
}
template <>
void save(ActionData::State& state, std::ostream& out) {
  // TODO(des): save state action
}

// Multiple
template <>
ActionData::Multiple load(std::istream& in) {
  ActionData::Multiple multiple;
  std::size_t size;
  in >> size >> std::ws;
  multiple.actions.reserve(size);
  for (std::size_t i = 0; i < size; i++) {
    multiple.actions.emplace_back(ActionData::load(in));
  }
  return multiple;
}
template <>
void save(ActionData::Multiple& multiple, std::ostream& out) {
  out << multiple.actions.size() << '\n';
  for (auto& action : multiple.actions) {
    action.save(out);
  }
}

};  // namespace

ActionData ActionData::load(std::istream& in) {
  return LoadImpl<decltype(data)>::load(in);
}

void ActionData::save(std::ostream& out) {
  std::visit(
      [&](auto&& data) { ::saveImpl<std::decay_t<decltype(data)>>(data, out); },
      data);
}
