#include "Editor.h"

#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <climits>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

#include "Common/LevelData.h"
#include "Resource/TileSet.h"

using namespace std::literals;

Editor::Editor()
    : grid(gridTileSize, {}, gridColor),
      sparseGrid(gridTileSize * sparseGridScale, {}, sparseGridColor) {
  sf::Vector2u screen_size = {1200, 840};
  sf::View view = {{0, 0}, sf::Vector2f{screen_size}};

  currentPosition = view.getCenter();
  window.create(sf::VideoMode{screen_size}, "Level editor");
  window.setFramerateLimit(60);
  if (!ImGui::SFML::Init(window)) {
    std::cerr << "failed to init imgui-sfml window" << std::endl;
    window.close();
  }
  setView(view);

  selection.setFillColor(sf::Color::Transparent);
  selection.setOutlineThickness(2.f);
  selection.setSize(gridTileSize);
  setSelect(SelectState::Tile);

  colliderFrame.setFillColor(sf::Color::Transparent);
  colliderFrame.setOutlineThickness(2.f);
  colliderFrame.setOutlineColor(colliderFrameColor);
  colliderFrame.setSize(gridTileSize);

  zeroPoint.setRadius(3.f);
  zeroPoint.setOrigin(zeroPoint.getGeometricCenter());
  zeroPoint.setPosition({0, 0});
}

void Editor::run() {
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      handleInput(*event);
      if (auto resized = event->getIf<sf::Event::Resized>()) {
        auto view = window.getView();
        view.setSize(sf::Vector2f{resized->size} * scale);
        setView(view);
      }
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    ImGui::SFML::Update(window, deltaClock.restart());

    widgets();
    window.clear(backgroundColor);
    draw();
    window.display();
  }
  ImGui::SFML::Shutdown();
}

void Editor::setSelect(SelectState state) {
  selectState = state;
  switch (selectState) {
    case SelectState::Tile:
      selection.setOutlineColor(tileSelectColor);
      selectionText.setFillColor(tileSelectColor);
      selectionText.setString("Tile");
      break;
    case SelectState::Collider:
      selection.setOutlineColor(colliderSelectColor);
      selectionText.setFillColor(colliderSelectColor);
      selectionText.setString("Collider");
      break;
    case SelectState::Entity:
      selection.setOutlineColor(entitySelectColor);
      selectionText.setFillColor(entitySelectColor);
      selectionText.setString("Entity");
      break;
  }
  auto bounds = selectionText.getLocalBounds();
  selectionText.setOrigin(
      {bounds.position.x, bounds.position.y + bounds.size.y + 5.f});
}

bool Editor::isSelected(SelectState state) { return selectState == state; }

void Editor::setTile(sf::Vector2i position, int tile) {
  auto element =
      std::make_pair(tile, sf::Sprite(*tileset, tileset->getTileRect(tile)));
  element.second.setPosition(
      sf::Vector2f{position}.componentWiseMul(gridTileSize));
  element.second.setScale(gridTileSize.componentWiseDiv(
      sf::Vector2f{element.second.getTextureRect().size}));
  tiles.insert_or_assign({position.x, position.y}, std::move(element));
}

void Editor::eraseTile(sf::Vector2i position) {
  tiles.erase({position.x, position.y});
}

void Editor::setCollider(sf::Vector2i position) {
  colliders.insert({position.x, position.y});
}

void Editor::eraseCollider(sf::Vector2i position) {
  colliders.erase({position.x, position.y});
}

void Editor::setView(sf::View view) {
  window.setView(view);
  grid.setView(view);
  sparseGrid.setView(view);
  if (!noBackground) background.setView(view);
}

void Editor::handleInput(sf::Event event) {
  handlePanning(event);
  handleScaling(event);
  if (auto position = sf::Mouse::getPosition(window);
      window.getViewport(window.getView()).contains(position) &&
      !widgetWindowRect.contains(position)) {
    handleKeys(event);
    handleTiles(event);
  }
}

void Editor::handlePanning(sf::Event event) {
  if (!sf::Keyboard::isKeyPressed(panKey)) return;
  if (auto mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>();
      mouseButtonPressed && mouseButtonPressed->button == panMouseButton &&
      !panButtonPressed) {
    panButtonPressed = true;
    lastPressPosition = mouseButtonPressed->position;
  } else if (auto mouseMoved = event.getIf<sf::Event::MouseMoved>();
             mouseMoved && panButtonPressed) {
    auto view = window.getView();
    view.setCenter(currentPosition +
                   panScaling * scale *
                       sf::Vector2f(mouseMoved->position - lastPressPosition));
    setView(view);
  } else if (auto mouseButtonReleased =
                 event.getIf<sf::Event::MouseButtonReleased>();
             mouseButtonReleased &&
             mouseButtonReleased->button == panMouseButton) {
    auto view = window.getView();
    panButtonPressed = false;
    currentPosition +=
        panScaling * scale *
        sf::Vector2f(mouseButtonReleased->position - lastPressPosition);
    view.setCenter(currentPosition);
    setView(view);
  }
}

void Editor::handleScaling(sf::Event event) {
  constexpr auto wheel = sf::Mouse::Wheel::Vertical;
  if (!sf::Keyboard::isKeyPressed(panKey)) return;
  if (auto mouseWheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>();
      mouseWheelScrolled && mouseWheelScrolled->wheel == wheel) {
    auto view = window.getView();
    float factor =
        mouseWheelScrolled->delta < 0 ? scalingFactor : invScalingFactor;
    auto dist = window.mapPixelToCoords(mouseWheelScrolled->position) -
                view.getCenter();
    scale *= factor;
    view.zoom(factor);
    view.move(dist * (1 - factor));
    currentPosition = view.getCenter();
    setView(view);
    selection.setOutlineThickness(factor * selection.getOutlineThickness());
    colliderFrame.setOutlineThickness(factor *
                                      colliderFrame.getOutlineThickness());
    selectionText.scale({factor, factor});
    zeroPoint.setRadius(factor * zeroPoint.getRadius());
    zeroPoint.setOrigin(zeroPoint.getGeometricCenter());
  }
}

void Editor::handleTiles(sf::Event event) {
  if (auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
    if (!widgetWindowRect.contains(mouseMoved->position)) {
      auto pos = window.mapPixelToCoords(mouseMoved->position);
      pos = {std::floor(pos.x / gridTileSize.x) * gridTileSize.x,
             std::floor(pos.y / gridTileSize.y) * gridTileSize.y};
      selection.setPosition(pos);
      selectionText.setPosition(pos);
    }
  }
  if (sf::Keyboard::isKeyPressed(panKey)) return;

  bool placeTile = false;
  bool removeTile = false;
  if (paintTiles) {
    placeTile = sf::Mouse::isButtonPressed(placeTileMouseButton);
    removeTile = sf::Mouse::isButtonPressed(removeTileMouseButton);
  } else if (auto mouseButtonReleased =
                 event.getIf<sf::Event::MouseButtonReleased>()) {
    placeTile = mouseButtonReleased->button == placeTileMouseButton;
    removeTile = mouseButtonReleased->button == removeTileMouseButton;
  }
  if (placeTile || removeTile) {
    auto coords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    auto position = sf::Vector2i(std::floor(coords.x / gridTileSize.x),
                                 std::floor(coords.y / gridTileSize.y));
    if (isSelected(SelectState::Collider)) {
      if (placeTile)
        setCollider(position);
      else
        eraseCollider(position);
    } else if (isSelected(SelectState::Tile) && selectedTile != -1) {
      if (placeTile)
        setTile(position, selectedTile);
      else
        eraseTile(position);
    }
  }
}

void Editor::handleKeys(sf::Event event) {
  auto keyReleased = event.getIf<sf::Event::KeyReleased>();
  if (!keyReleased) return;
  if (keyReleased->code == sf::Keyboard::Key::Q) paintTiles = !paintTiles;
  if (keyReleased->code == sf::Keyboard::Key::E)
    setSelect(SelectState::Collider);
  if (!tileset) return;
  if (keyReleased->code == sf::Keyboard::Key::Tab && selectedTile != -1) {
    int nextTile = selectedTile + (keyReleased->shift ? -1 : 1);
    selectedTile = std::min<int>(
        std::max(nextTile, 0), (tileset->getSize().x / tileset->tileSize) *
                                   (tileset->getSize().y / tileset->tileSize));
  }
}

static bool CustomSelectable(const char* label, bool selected,
                             ImGuiCol bg_color, ImGuiSelectableFlags flags = 0,
                             const ImVec2& size_arg = {0, 0}) {
  // https://github.com/ocornut/imgui/issues/4719
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->ChannelsSplit(2);
  draw_list->ChannelsSetCurrent(1);
  bool result = ImGui::Selectable(label, selected, flags, size_arg);
  if (!ImGui::IsItemHovered() && !ImGui::IsItemActive() && !selected) {
    draw_list->ChannelsSetCurrent(0);
    ImVec2 p_min = ImGui::GetItemRectMin();
    ImVec2 p_max = ImGui::GetItemRectMax();
    ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max,
                                              ImGui::GetColorU32(bg_color));
  }
  draw_list->ChannelsMerge();
  return result;
}

void Editor::widgets() {
  static constexpr ImVec2 selectableSize = {50, 50};

  ImGui::Begin("Resources");
  widgetWindowRect.position = ImGui::GetWindowPos();
  widgetWindowRect.size = ImGui::GetWindowSize();
  ImGui::Checkbox("Paint tiles (Q)", &paintTiles);
  if (CustomSelectable("Collider (E)", isSelected(SelectState::Collider),
                       ImGuiCol_MenuBarBg))
    setSelect(SelectState::Collider);

  ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
  if (ImGui::BeginTabBar("ResourcesTabBar", tab_bar_flags)) {
    if (ImGui::BeginTabItem("Background")) {
      backgroundWidget();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("TileSet")) {
      tilesetLoadWidget();
      tilesetSelectWidget();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Entities")) {
      entitiesWidget();
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }

  loadWidget();
  saveWidget();
  ImGui::End();
}

void Editor::backgroundWidget() {
  ImGui::Checkbox("No background", &noBackground);
  if (noBackground) ImGui::BeginDisabled();
  ImGui::InputText("Path", &backgroundPathBuf);

  if (ImGui::DragFloat2("Texture size", backgroundSize, 8.f, 1.0f, FLT_MAX,
                        "%.2f") &&
      backgroundTexture) {
    background.setTextureSize({backgroundSize[0], backgroundSize[1]});
    background.setView(window.getView());
  }
  if (ImGui::Checkbox("Moving", &backgroundMoving)) {
    background.setMoving(backgroundMoving);
    background.setView(window.getView());
  }
  if (ImGui::Checkbox("Repeated", &backgroundRepeated) && backgroundTexture) {
    backgroundTexture->setRepeated(backgroundRepeated);
  }
  if (ImGui::Button("Load")) {
    try {
      loadBackgroundFile();
    } catch (sf::Exception e) {
      backgroundPopupText = e.what();
      ImGui::OpenPopup("BackgroundLoadPopup");
    } catch (std::filesystem::filesystem_error e) {
      backgroundPopupText = e.what();
      ImGui::OpenPopup("BackgroundLoadPopup");
    }
  }
  if (ImGui::BeginPopup("BackgroundLoadPopup")) {
    ImGui::Text("%s", backgroundPopupText.c_str());
    ImGui::EndPopup();
  }
  if (noBackground) ImGui::EndDisabled();
}

void Editor::tilesetLoadWidget() {
  ImGui::InputText("Path", &tilesetPathBuf);
  ImGui::DragInt("Tile size", &tilesetTileSize, 1.f, 1, INT_MAX);
  if (ImGui::Button("Load")) {
    try {
      loadTilemapFile();
    } catch (sf::Exception e) {
      tilesetPopupText = e.what();
      ImGui::OpenPopup("TilesetLoadPopup");
    } catch (std::filesystem::filesystem_error e) {
      tilesetPopupText = e.what();
      ImGui::OpenPopup("TilesetLoadPopup");
    }
  }
  if (ImGui::BeginPopup("TilesetLoadPopup")) {
    ImGui::Text("%s", tilesetPopupText.c_str());
    ImGui::EndPopup();
  }
}

void Editor::tilesetSelectWidget() {
  if (!tileset) return;
  auto [w, h] = tileset->getSize() / tileset->tileSize;

  static constexpr ImVec2 selectableSize = {40, 40};
  static constexpr float selectableImagePadding = 0.f;

  ImGui::SeparatorText("Tiles");

  ImGui::BeginChild("tilesetSelect", {0, 0},
                    ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

  if (CustomSelectable(
          "Empty", isSelected(SelectState::Tile) && selectedTile == -1,
          ImGuiCol_MenuBarBg, ImGuiSelectableFlags_None, selectableSize)) {
    if (!isSelected(SelectState::Tile)) setSelect(SelectState::Tile);
    selectedTile = -1;
  }

  auto startPosX = ImGui::GetCursorPosX();
  for (unsigned i = 0; i < h; i++) {
    for (unsigned j = 0; j < w; j++) {
      auto idx = i * h + j;
      ImGui::PushID(idx);
      selectableTileSprite->setTextureRect(tileset->getTileRect(idx));
      auto cursorPos = ImGui::GetCursorPos();
      if (CustomSelectable(
              "", isSelected(SelectState::Tile) && selectedTile == idx,
              ImGuiCol_MenuBarBg, ImGuiSelectableFlags_None, selectableSize)) {
        if (!isSelected(SelectState::Tile)) setSelect(SelectState::Tile);
        selectedTile = idx;
      }
      ImGui::SameLine();
      auto afterCursorPos = ImGui::GetCursorPos();
      ImGui::SetCursorPos({cursorPos.x + selectableImagePadding,
                           cursorPos.y + selectableImagePadding});
      ImGui::Image(*selectableTileSprite,
                   sf::Vector2f{selectableSize.x - 2 * selectableImagePadding,
                                selectableSize.y - 2 * selectableImagePadding});
      if (j + 1 < w) {
        ImGui::SameLine();
        ImGui::SetCursorPos(afterCursorPos);
      }
      ImGui::PopID();
    }
  }
  ImGui::EndChild();
}

void Editor::entitiesWidget() { ImGui::Text("select entities"); }

void Editor::loadWidget() {
  ImGui::SeparatorText("Loading");
  ImGui::InputText("Path##load", loadPathBuf, sizeof(loadPathBuf));
  if (ImGui::Button("Load")) {
    try {
      load(loadPathBuf);
    } catch (const std::filesystem::filesystem_error& e) {
      loadPopupText = "failed to load level data from " +
                      std::string(loadPathBuf) + ": " + e.what();
      ImGui::OpenPopup("LoadPopup");
    }
  }
  if (ImGui::BeginPopup("LoadPopup")) {
    ImGui::Text("%s", loadPopupText.c_str());
    ImGui::EndPopup();
  }
}

void Editor::saveWidget() {
  ImGui::SeparatorText("Saving");
  ImGui::InputText("Path##save", savePathBuf, sizeof(savePathBuf));
  ImGui::DragFloat("Tile size", &saveTileSize, 0.25f, 1.0f, FLT_MAX, "%.2f");
  if (ImGui::Button("Save")) {
    try {
      save(savePathBuf);
    } catch (const std::filesystem::filesystem_error& e) {
      savePopupText = "failed to save level data to " +
                      std::string(savePathBuf) + ": " + e.what();
      ImGui::OpenPopup("SavePopup");
    }
  }
  if (ImGui::BeginPopup("SavePopup")) {
    ImGui::Text("%s", savePopupText.c_str());
    ImGui::EndPopup();
  }
}

void Editor::load(const std::filesystem::path& filename) {
  std::ifstream in(filename);
  try {
    LevelData data = LevelData::load(in);
    loadBackground(data.background);
    loadTilemap(data.tilemap);
    loadEntities(data.entities);
    loadPopupText = "Loaded successfully";
  } catch (const std::filesystem::filesystem_error& e) {
    loadPopupText =
        "failed to load level data from " + filename.string() + ": " + e.what();
  } catch (const std::runtime_error& e) {
    loadPopupText =
        "failed to load level data from " + filename.string() + ": " + e.what();
  }
  ImGui::OpenPopup("LoadPopup");
}

void Editor::loadBackgroundFile() {
  backgroundTexture = std::make_unique<sf::Texture>(backgroundPathBuf);
  backgroundTexturePath = backgroundPathBuf;
  background.setTexture(backgroundTexture.get(),
                        {backgroundSize[0], backgroundSize[1]});
  backgroundTexture->setRepeated(backgroundRepeated);
  background.setMoving(backgroundMoving);
  background.setView(window.getView());
}

void Editor::loadBackground(std::optional<LevelData::BackgroundData>& data) {
  if (!data) {
    background.unsetTexture();
    backgroundTexture.reset();
    backgroundTexturePath.clear();
    return;
  }
  auto prevNoBackground = noBackground;
  auto prevBackgroundPathBuf = backgroundPathBuf;
  float prevBackgroundSize[2] = {backgroundSize[0], backgroundSize[1]};
  auto prevBackgroundMoving = backgroundMoving;
  auto prevBackgroundRepeated = backgroundRepeated;
  try {
    noBackground = false;
    backgroundPathBuf = data->texturePath;
    backgroundSize[0] = data->size.x;
    backgroundSize[1] = data->size.y;
    backgroundMoving = data->moving;
    backgroundRepeated = data->repeated;
    loadBackgroundFile();
  } catch (...) {
    noBackground = prevNoBackground;
    backgroundPathBuf = prevBackgroundPathBuf;
    backgroundSize[0] = prevBackgroundSize[0];
    backgroundSize[1] = prevBackgroundSize[1];
    backgroundMoving = prevBackgroundMoving;
    backgroundRepeated = prevBackgroundRepeated;
    throw;
  }
}

void Editor::loadTilemapFile() {
  tileset = std::make_unique<TileSet>(tilesetPathBuf, tilesetTileSize);
  tilesetPath = tilesetPathBuf;
  selectedTile = -1;
  tiles.clear();
  if (selectableTileSprite) {
    selectableTileSprite->setTexture(*tileset);
  } else {
    selectableTileSprite = std::make_unique<sf::Sprite>(*tileset);
  }
}

void Editor::loadTilemap(LevelData::TilemapData& data) {
  if (data.noTileset) {
    tileset.reset();
    tilesetPath.clear();
    selectedTile = -1;
    tiles.clear();
  } else {
    auto prevTilesetTileSize = tilesetTileSize;
    auto prevTilesetPathBuf = tilesetPathBuf;
    try {
      tilesetTileSize = data.tilesetTileSize;
      tilesetPathBuf = data.tilesetPath;
      loadTilemapFile();
    } catch (...) {
      tilesetTileSize = prevTilesetTileSize;
      tilesetPathBuf = prevTilesetPathBuf;
      throw;
    }
  }
  colliders.clear();
  for (int i = 0; i < data.height; i++) {
    for (int j = 0; j < data.width; j++) {
      if (!data.noTileset) setTile({i, j}, data.tiles[i * data.width + j]);
      if (data.colliders[i * data.width + j]) setCollider({i, j});
    }
  }
}

void Editor::loadEntities(std::vector<LevelData::EntityData>& data) {
  // TODO: uhhh...
}

void Editor::save(const std::filesystem::path& filename) {
  std::ofstream out(filename);
  try {
    LevelData data;
    saveBackground(data);
    sf::Vector2i origin = saveTilemap(data);
    saveEntities(data, origin);
    data.save(out);
    savePopupText = "Saved successfully";
  } catch (const std::runtime_error& e) {
    savePopupText =
        "failed to save level data to " + filename.string() + ": " + e.what();
  }
  ImGui::OpenPopup("SavePopup");
}

void Editor::saveBackground(LevelData& data) {
  if (noBackground || backgroundTexturePath.empty()) return;
  data.background->texturePath = backgroundTexturePath;
  data.background->repeated = backgroundRepeated;
  data.background->moving = backgroundMoving;
  data.background->size = {backgroundSize[0], backgroundSize[1]};
}

sf::Vector2i Editor::saveTilemap(LevelData& data) {
  auto [min_x_it, max_x_it] = std::minmax_element(
      tiles.begin(), tiles.end(), [](const auto& a, const auto& b) {
        return a.first.first < b.first.first;
      });
  auto [min_y_it, max_y_it] = std::minmax_element(
      tiles.begin(), tiles.end(), [](const auto& a, const auto& b) {
        return a.first.second < b.first.second;
      });
  auto [min_c_x_it, max_c_x_it] = std::minmax_element(
      colliders.begin(), colliders.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });
  auto [min_c_y_it, max_c_y_it] = std::minmax_element(
      colliders.begin(), colliders.end(),
      [](const auto& a, const auto& b) { return a.second < b.second; });
  int min_x = std::min(
      tiles.empty() ? std::numeric_limits<int>::max() : min_x_it->first.first,
      colliders.empty() ? std::numeric_limits<int>::max() : min_c_x_it->first);
  int max_x = std::max(
      tiles.empty() ? std::numeric_limits<int>::min() : max_x_it->first.first,
      colliders.empty() ? std::numeric_limits<int>::min() : max_c_x_it->first);
  int min_y = std::min(
      tiles.empty() ? std::numeric_limits<int>::max() : min_y_it->first.second,
      colliders.empty() ? std::numeric_limits<int>::max() : min_c_y_it->second);
  int max_y = std::max(
      tiles.empty() ? std::numeric_limits<int>::min() : max_y_it->first.second,
      colliders.empty() ? std::numeric_limits<int>::min() : max_c_y_it->second);
  int width = tiles.empty() && colliders.empty() ? 0 : max_x - min_x + 1;
  int height = tiles.empty() && colliders.empty() ? 0 : max_y - min_y + 1;
  data.tilemap.noTileset = !tileset || tiles.empty();
  data.tilemap.tileSize = saveTileSize;
  if (!data.tilemap.noTileset) {
    data.tilemap.tilesetPath = tilesetPath;
    data.tilemap.tilesetTileSize = tileset->tileSize;
    data.tilemap.tiles.resize(width * height);
  }
  data.tilemap.colliders.resize(width * height);
  data.tilemap.width = width;
  data.tilemap.height = height;
  for (int i = 0, y = min_y; i < height; i++, y++) {
    for (int j = 0, x = min_x; j < width; j++, x++) {
      if (!data.tilemap.noTileset) {
        auto it = tiles.find({x, y});
        data.tilemap.tiles[i * width + j] =
            it == tiles.end() ? -1 : it->second.first;
      }
      data.tilemap.colliders[i * width + j] = colliders.count({x, y});
    }
  }
  return {min_x, min_y};
}

void Editor::saveEntities(LevelData& data, sf::Vector2i origin) {
  // TODO: ugh...
}

void Editor::draw() {
  if (!noBackground) window.draw(background);
  window.draw(grid);
  window.draw(sparseGrid);
  for (auto& [coords, tile] : tiles) {
    window.draw(tile.second);
  }
  for (auto& coords : colliders) {
    colliderFrame.setPosition(
        {coords.first * gridTileSize.x, coords.second * gridTileSize.y});
    window.draw(colliderFrame);
  }
  window.draw(zeroPoint);
  window.draw(selection);
  window.draw(selectionText);
  ImGui::SFML::Render(window);
}
