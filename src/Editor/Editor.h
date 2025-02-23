#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System.hpp>
#include <map>
#include <memory>
#include <set>

#include "Editor/Grid.h"
#include "Entity/Background.h"
#include "Resource/TileSet.h"

class Editor {
 private:
  // Keybinds
  static constexpr auto panKey = sf::Keyboard::Key::LShift;
  static constexpr auto panMouseButton = sf::Mouse::Button::Left;
  static constexpr auto placeTileMouseButton = sf::Mouse::Button::Left;
  static constexpr auto removeTileMouseButton = sf::Mouse::Button::Right;

  // Colors
  static constexpr sf::Color backgroundColor = sf::Color(10, 10, 10);
  static constexpr sf::Color gridColor = {20, 20, 20};
  static constexpr sf::Color sparseGridColor = {50, 50, 50};
  static constexpr sf::Color tileSelectionColor = {0, 255, 0, 128};
  static constexpr sf::Color tileSelectionColliderColor = {0, 128, 128, 128};
  static constexpr sf::Color tileColliderColor = {0, 0, 255, 128};

  // Variables
  float panScaling = -1.2f;
  float scalingFactor = 1.1f;
  float sparseGridScale = 4.f;
  sf::Vector2f gridTileSize = {32, 32};

  // Calculated variables
  float invScalingFactor = 1.f / scalingFactor;

  // Controllable state
  bool paintTiles = true;
  bool colliderSelected = false;
  bool noBg = true;
  float backgroundSize[2] = {512.f, 512.f};
  bool backgroundMoving = true;
  bool backgroundRepeated = true;
  float tileSize = 64.f;

  // Inner state
  bool pressed = false;
  float scale = 1.f;
  int selectedTile = -1;
  std::string savePopupText;
  std::filesystem::path tilesetPath;
  std::filesystem::path backgroundPath;
  sf::Vector2i lastPressPosition;
  sf::Vector2f currentPosition;
  sf::IntRect widgetWindowRect;
  std::map<std::pair<int, int>, std::pair<int, sf::Sprite>> sparseTileMap;
  std::set<std::pair<int, int>> sparseTileColliders;

  // Members
  sf::RenderWindow window;
  sf::Clock deltaClock;
  Background background;
  std::unique_ptr<sf::Texture> backgroundTexture;
  Grid grid, sparseGrid;
  sf::RectangleShape tileSelection, tileCollider;
  sf::CircleShape zeroPoint;
  std::unique_ptr<TileSet> tileset;
  std::unique_ptr<sf::Sprite> sprite;

 public:
  Editor();
  void run();

 private:
  void setView(sf::View view);
  void handleInput(sf::Event event);
  void handlePanning(sf::Event event);
  void handleScaling(sf::Event event);
  void handleTiles(sf::Event event);
  void setTile(sf::Vector2i position);
  void removeTile(sf::Vector2i position);
  void handleKeys(sf::Event event);
  void widgets();
  void backgroundWidget();
  void tilesetLoadWidget();
  void tilesetSelectWidget();
  void entitiesWidget();
  void saveWidget();
  void save(const std::filesystem::path& path);
  void saveBackground(std::ofstream& out);
  sf::Vector2i saveTilemap(std::ofstream& out);
  void draw();
};
