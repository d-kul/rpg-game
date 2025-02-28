#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
#include <memory>
#include <set>

#include "Common/Data/Level.h"
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
  static constexpr sf::Color colliderFrameColor = {0, 0, 255, 128};
  static constexpr sf::Color tileSelectColor = {0, 255, 0, 128};
  static constexpr sf::Color colliderSelectColor = {0, 128, 128, 128};
  static constexpr sf::Color entitySelectColor = {255, 80, 80, 128};

  // Variables
  float panScaling = -1.2f;
  float scalingFactor = 1.1f;
  float sparseGridScale = 4.f;
  sf::Vector2f gridTileSize = {32, 32};

  // Calculated variables
  float invScalingFactor = 1.f / scalingFactor;

  // Controllable state
  bool paintTiles = true;
  enum class SelectState {
    Tile,
    Collider,
    Entity
  } selectState = SelectState::Tile;
  int selectedTile = -1;

  bool noBackground = true;
  std::string backgroundPathBuf = "resources/images/bg/space_fumo.png";
  float backgroundSize[2] = {512.f, 512.f};
  bool backgroundMoving = true;
  bool backgroundRepeated = true;
  std::string backgroundPopupText;

  int tilesetTileSize = 32;
  std::string tilesetPathBuf = "resources/images/tilesets/indoors.png";
  std::string tilesetPopupText;

  char loadPathBuf[128] = "resources/data/levels/level.txt";

  char savePathBuf[128] = "resources/data/levels/level.txt";
  float saveTileSize = 64.f;

  // Inner state
  bool panButtonPressed = false;
  float scale = 1.f;
  std::string loadPopupText, savePopupText;
  sf::Vector2i lastPressPosition;
  sf::Vector2f currentPosition;
  sf::IntRect widgetWindowRect;
  std::map<std::pair<int, int>, std::pair<int, sf::Sprite>> tiles;
  std::set<std::pair<int, int>> colliders;

  // Members
  sf::RenderWindow window;
  sf::Clock deltaClock;
  sf::Font font{"resources/fonts/DroidSansMono.ttf"};

  // Gizmos
  Grid grid, sparseGrid;
  sf::CircleShape zeroPoint;
  sf::RectangleShape selection;
  sf::Text selectionText{font, "", 25};
  sf::RectangleShape colliderFrame;
  // sf::RectangleShape playerRect;
  // sf::Text playerText;

  Background background;
  std::unique_ptr<sf::Texture> backgroundTexture;
  std::filesystem::path backgroundTexturePath;

  std::unique_ptr<TileSet> tileset;
  std::filesystem::path tilesetPath;

  // Auxillary
  std::unique_ptr<sf::Sprite> selectableTileSprite;

 public:
  Editor();
  void run();

 private:
  void setSelect(SelectState state);
  bool isSelected(SelectState state);

  void setTile(sf::Vector2i position, int tile);
  void eraseTile(sf::Vector2i position);
  void setCollider(sf::Vector2i position);
  void eraseCollider(sf::Vector2i position);

  void setView(sf::View view);

  void handleInput(sf::Event event);
  void handlePanning(sf::Event event);
  void handleScaling(sf::Event event);
  void handleTiles(sf::Event event);
  void handleKeys(sf::Event event);

  void widgets();

  void backgroundWidget();
  void tilesetLoadWidget();
  void tilesetSelectWidget();
  void entitiesWidget();

  void loadWidget();
  void saveWidget();

  void load(const std::filesystem::path& path);

  void loadBackgroundFile();
  void loadBackground(std::optional<LevelData::Background>& data);

  void loadTilemapFile();
  void loadTilemap(LevelData::Tilemap& data);

  void loadEntities(std::vector<EntityData>& data);

  void save(const std::filesystem::path& path);

  void saveBackground(LevelData& data);
  sf::Vector2i saveTilemap(LevelData& data);
  void saveEntities(LevelData& data, sf::Vector2i origin);

  void draw();
};
