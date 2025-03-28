#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <list>
#include <map>
#include <memory>
#include <set>

#include "Common/Data/Level.h"
#include "Editor/Grid.h"
#include "Entity/Background.h"
#include "Resource/TileSet.h"

class Editor {
 private:
  // Data types
  struct Entity;

  struct Action {
    ActionData data;
    std::string pathBuf;
    std::string intBuf;
    Action* actionBuf = nullptr;
    Action** holderBuf = nullptr;
    Entity* holderEntityBuf = nullptr;
    Action* next = nullptr;
  };

  struct Entity {
    struct Drawable {};

    struct Interactible {
      Action* action = nullptr;
    };

    struct Player {};

    struct Character {
      Drawable drawable;
      Interactible interactible;
    };

    struct Prop {
      Drawable drawable;
      Interactible interactible;
    };

    struct Trigger {
      Interactible interactible;
    };

    Entity(const sf::Font& font, Player player)
        : position{0, 0}, data{player}, frame{}, text{font, "Player", 20} {}

    Entity(const sf::Font& font, Character character)
        : position{0, 0},
          data{character},
          frame{},
          text{font, "Character", 20} {}

    Entity(const sf::Font& font, Prop prop)
        : position{0, 0}, data{prop}, frame{}, text{font, "Prop", 20} {}

    Entity(const sf::Font& font, Trigger trigger)
        : position{0, 0}, data{trigger}, frame{}, text{font, "Trigger", 20} {}

    sf::Vector2i position;
    std::variant<Player, Character, Prop, Trigger> data;

    sf::RectangleShape frame;
    sf::Text text;
  };

  // Keybinds
  static constexpr auto panKey = sf::Keyboard::Key::LShift;
  static constexpr auto panMouseButton = sf::Mouse::Button::Left;
  static constexpr auto placeTileMouseButton = sf::Mouse::Button::Left;
  static constexpr auto removeTileMouseButton = sf::Mouse::Button::Right;
  static constexpr auto moveUpKey = sf::Keyboard::Key::Up;
  static constexpr auto moveDownKey = sf::Keyboard::Key::Down;

  // Colors
  static constexpr sf::Color backgroundColor = sf::Color(10, 10, 10);
  static constexpr sf::Color gridColor = {20, 20, 20};
  static constexpr sf::Color sparseGridColor = {50, 50, 50};
  static constexpr sf::Color noneSelectColor = {100, 100, 100, 200};
  static constexpr sf::Color tileSelectColor = {0, 255, 0, 200};
  static constexpr sf::Color colliderSelectColor = {0, 128, 128, 200};
  static constexpr sf::Color entitySelectColor = {128, 64, 64, 200};
  static constexpr sf::Color colliderFrameColor = {0, 0, 255, 128};
  static constexpr sf::Color selectedEntityColor = {255, 0, 128, 220};
  static constexpr sf::Color entityColor = {255, 0, 0, 220};

  // Variables
  float panScaling = -1.2f;
  float scalingFactor = 1.1f;
  float sparseGridScale = 4.f;
  sf::Vector2f gridTileSize = {32, 32};

  // Calculated variables
  float invScalingFactor = 1.f / scalingFactor;

  // Controllable state
  bool paintTiles = true;
  enum class SelectState { None, Tile, Collider, Entity } selectState;
  int selectedTile = -1;
  Entity* selectedEntity = nullptr;

  bool noBackground = true;
  std::string backgroundPathBuf = "resources/images/bg/space_fumo.png";
  float backgroundSize[2] = {512.f, 512.f};
  bool backgroundMoving = true;
  bool backgroundRepeated = true;
  std::string backgroundPopupText;

  int tilesetTileSize = 32;
  std::string tilesetPathBuf = "resources/images/tilesets/indoors.png";
  std::string tilesetPopupText;

  char levelPathBuf[128] = "resources/data/levels/level.txt";
  float levelTileSize = 64.f;
  std::string levelPopupText;

  std::list<Entity> entities;
  std::list<Action> actions;
  Action* startAction = nullptr;
  bool toChangeScale = false;

  // Inner state
  bool panButtonPressed = false;
  bool moveUpButtonPressed = false;
  bool moveDownButtonPressed = false;
  bool moveUpButtonClicked = false;
  bool moveDownButtonClicked = false;
  float scale = 1.f;
  sf::Vector2i lastPressPosition;
  sf::Vector2f currentPosition;
  sf::IntRect widgetWindowRect;
  std::map<std::pair<int, int>, std::pair<int, sf::Sprite>> tiles;
  std::set<std::pair<int, int>> colliders;
  std::unordered_map<Action*, std::set<Action**>> actionHolders;
  std::unordered_map<Action**, std::set<Action*>> holderHolders;
  Action* highlightedAction = nullptr;
  Action* currentHighlightedAction = nullptr;
  Entity* highlightedHolderEntity = nullptr;

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

  Background background;
  std::unique_ptr<sf::Texture> backgroundTexture;
  std::filesystem::path backgroundTexturePath;

  std::unique_ptr<sf::Texture> tilesetTexture;
  TileSet tileset;
  std::filesystem::path tilesetPath;

  // Auxillary
  std::unique_ptr<sf::Sprite> selectableTileSprite;

 public:
  Editor();
  void run();

 private:
  void setSelect(SelectState state);
  bool isSelected(SelectState state);
  void selectEntity(Entity* entity);
  void prepareEntity(Entity& entity);

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

  void spriteSheetsWidget();

  void actionsWidget();
  void actionTarget(Action*& action, const char* descriptor = "");
  void holderTarget(Action& target, const char* descriptor = "");

  void levelWidget();

  void load(const std::filesystem::path& path);
  void save(const std::filesystem::path& path);

  void loadBackgroundFile();
  void loadBackground(std::optional<LevelData::Background>& data);
  void saveBackground(LevelData& data);

  void loadTilesetFile();
  void loadTilemap(LevelData::Tilemap& data);
  sf::Vector2i saveTilemap(LevelData& data);

  void loadEntities(std::vector<EntityData>& data);
  void saveEntities(std::vector<EntityData>& data, sf::Vector2i origin);

  void loadActions(std::vector<ActionData>& data);
  void saveActions(std::vector<ActionData>& data);

  void bindActionRefs(LevelData& data);
  void bindActionIndices(LevelData& data);

  void draw();
};
