#include "Editor.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <climits>
#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

#include "Core/Logger.h"
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

  tileSelection.setFillColor(sf::Color::Transparent);
  tileSelection.setOutlineThickness(2.f);
  tileSelection.setOutlineColor(tileSelectionColor);
  tileSelection.setSize(gridTileSize);

  tileCollider.setFillColor(sf::Color::Transparent);
  tileCollider.setOutlineThickness(2.f);
  tileCollider.setOutlineColor(tileColliderColor);
  tileCollider.setSize(gridTileSize);

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

void Editor::setView(sf::View view) {
  window.setView(view);
  grid.setView(view);
  sparseGrid.setView(view);
  if (!noBg) background.setView(view);
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
      !pressed) {
    pressed = true;
    lastPressPosition = mouseButtonPressed->position;
  } else if (auto mouseMoved = event.getIf<sf::Event::MouseMoved>();
             mouseMoved && pressed) {
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
    pressed = false;
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
    tileSelection.setOutlineThickness(factor *
                                      tileSelection.getOutlineThickness());
    tileCollider.setOutlineThickness(factor *
                                     tileCollider.getOutlineThickness());
    zeroPoint.setRadius(factor * zeroPoint.getRadius());
    zeroPoint.setOrigin(zeroPoint.getGeometricCenter());
  }
}

void Editor::handleTiles(sf::Event event) {
  tileSelection.setOutlineColor(colliderSelected ? tileSelectionColliderColor
                                                 : tileSelectionColor);
  if (auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
    if (!widgetWindowRect.contains(mouseMoved->position)) {
      auto pos = window.mapPixelToCoords(mouseMoved->position);
      pos = {std::floor(pos.x / gridTileSize.x) * gridTileSize.x,
             std::floor(pos.y / gridTileSize.y) * gridTileSize.y};
      tileSelection.setPosition(pos);
    }
  }
  if (sf::Keyboard::isKeyPressed(panKey)) return;

  if (paintTiles) {
    auto position = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(placeTileMouseButton)) {
      setTile(position);
    } else if (sf::Mouse::isButtonPressed(removeTileMouseButton)) {
      removeTile(position);
    }
    return;
  }
  if (auto mouseButtonReleased =
          event.getIf<sf::Event::MouseButtonReleased>()) {
    if (mouseButtonReleased->button == placeTileMouseButton) {
      setTile(mouseButtonReleased->position);
    } else if (mouseButtonReleased->button == removeTileMouseButton) {
      removeTile(mouseButtonReleased->position);
    }
  }
}

void Editor::setTile(sf::Vector2i position) {
  auto position_view = window.mapPixelToCoords(position);
  auto tile_coords = sf::Vector2i(std::floor(position_view.x / gridTileSize.x),
                                  std::floor(position_view.y / gridTileSize.y));
  if (colliderSelected) {
    sparseTileColliders.insert({tile_coords.x, tile_coords.y});
    return;
  } else if (!tileset)
    return;
  if (selectedTile == -1) return;
  auto position_snapped =
      sf::Vector2f{tile_coords}.componentWiseMul(gridTileSize);
  auto elem = std::make_pair(
      selectedTile, sf::Sprite(*tileset, tileset->getTileRect(selectedTile)));
  elem.second.setPosition(position_snapped);
  elem.second.setScale(gridTileSize.componentWiseDiv(
      sf::Vector2f{elem.second.getTextureRect().size}));
  sparseTileMap.insert_or_assign({tile_coords.x, tile_coords.y},
                                 std::move(elem));
}

void Editor::removeTile(sf::Vector2i position) {
  auto position_view = window.mapPixelToCoords(position);
  auto tile_coords = sf::Vector2i(std::floor(position_view.x / gridTileSize.x),
                                  std::floor(position_view.y / gridTileSize.y));
  if (colliderSelected) {
    sparseTileColliders.erase({tile_coords.x, tile_coords.y});
    return;
  }
  sparseTileMap.erase({tile_coords.x, tile_coords.y});
}

void Editor::handleKeys(sf::Event event) {
  auto keyReleased = event.getIf<sf::Event::KeyReleased>();
  if (!keyReleased) return;
  if (keyReleased->code == sf::Keyboard::Key::Q) paintTiles = !paintTiles;
  if (keyReleased->code == sf::Keyboard::Key::E)
    colliderSelected = !colliderSelected;
  if (!tileset) return;
  if (keyReleased->code == sf::Keyboard::Key::Tab && selectedTile != -1) {
    int nextTile = selectedTile + (keyReleased->shift ? -1 : 1);
    selectedTile = std::min<int>(
        std::max(nextTile, 0), (tileset->getSize().x / tileset->tileSize) *
                                   (tileset->getSize().y / tileset->tileSize));
  }
}

void Editor::widgets() {
  ImGui::Begin("Resources");
  widgetWindowRect.position = ImGui::GetWindowPos();
  widgetWindowRect.size = ImGui::GetWindowSize();
  ImGui::Checkbox("Paint tiles (Q)", &paintTiles);
  ImGui::Checkbox("Collider (E)", &colliderSelected);

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

  saveWidget();
  ImGui::End();
}

void Editor::backgroundWidget() {
  static char pathBuf[128] = "resources/images/bg/space_fumo.png";
  static std::string popupText;
  ImGui::Checkbox("No background", &noBg);
  if (noBg) ImGui::BeginDisabled();
  ImGui::InputText("Path", pathBuf, sizeof(pathBuf));
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
      backgroundTexture = std::make_unique<sf::Texture>(pathBuf);
      backgroundPath = pathBuf;
      background.setTexture(backgroundTexture.get(),
                            {backgroundSize[0], backgroundSize[1]},
                            backgroundRepeated);
      background.setMoving(backgroundMoving);
      background.setView(window.getView());
    } catch (sf::Exception e) {
      popupText = e.what();
      ImGui::OpenPopup("BgLoadPopup");
    } catch (std::filesystem::filesystem_error e) {
      popupText = e.what();
      ImGui::OpenPopup("BgLoadPopup");
    }
  }
  if (ImGui::BeginPopup("BgLoadPopup")) {
    ImGui::Text("%s", popupText.c_str());
    ImGui::EndPopup();
  }
  if (noBg) ImGui::EndDisabled();
}

void Editor::tilesetLoadWidget() {
  static int tileSize = 32;
  static char pathBuf[128] = "resources/images/tilesets/indoors.png";
  static std::string popupText;
  ImGui::InputText("Path", pathBuf, sizeof(pathBuf));
  ImGui::DragInt("Tile size", &tileSize, 1.f, 1, INT_MAX);
  if (ImGui::Button("Load")) {
    try {
      tileset = std::make_unique<TileSet>(pathBuf, tileSize);
      sprite = std::make_unique<sf::Sprite>(*tileset);
      tilesetPath = pathBuf;
      sparseTileMap.clear();
    } catch (sf::Exception e) {
      popupText = e.what();
      ImGui::OpenPopup("LoadPopup");
    } catch (std::filesystem::filesystem_error e) {
      popupText = e.what();
      ImGui::OpenPopup("LoadPopup");
    }
  }
  if (ImGui::BeginPopup("LoadPopup")) {
    ImGui::Text("%s", popupText.c_str());
    ImGui::EndPopup();
  }
}

void Editor::tilesetSelectWidget() {
  if (!tileset) return;
  auto [w, h] = tileset->getSize() / tileset->tileSize;

  static ImVec2 tileSize = {40, 40};
  static float tilePadding = 0.f;

  ImGui::SeparatorText("Tiles");

  if (colliderSelected) ImGui::BeginDisabled();

  ImGui::BeginChild("tilesetSelect", {0, 0},
                    ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
  auto startPosX = ImGui::GetCursorPosX();
  for (unsigned i = 0; i < h; i++) {
    for (unsigned j = 0; j < w; j++) {
      auto idx = i * h + j;
      ImGui::PushID(idx);
      sprite->setTextureRect(tileset->getTileRect(idx));
      auto cursorPos = ImGui::GetCursorPos();
      if (ImGui::Selectable("", !colliderSelected && selectedTile == idx,
                            ImGuiSelectableFlags_None, tileSize)) {
        selectedTile = idx;
      }
      ImGui::SameLine();
      auto afterCursorPos = ImGui::GetCursorPos();
      ImGui::SetCursorPos(
          {cursorPos.x + tilePadding, cursorPos.y + tilePadding});
      ImGui::Image(*sprite, sf::Vector2f{tileSize.x - 2 * tilePadding,
                                         tileSize.y - 2 * tilePadding});
      if (j + 1 < w) {
        ImGui::SameLine();
        ImGui::SetCursorPos(afterCursorPos);
      }
      ImGui::PopID();
    }
  }
  ImGui::EndChild();

  if (colliderSelected) ImGui::EndDisabled();
}

void Editor::entitiesWidget() { ImGui::Text("select entities"); }

void Editor::saveWidget() {
  static char outPathBuf[128] = "resources/data/levels/level.txt";
  ImGui::SeparatorText("Saving");
  ImGui::InputText("Path", outPathBuf, sizeof(outPathBuf));
  ImGui::DragFloat("Tile size", &tileSize, 0.25f, 1.0f, FLT_MAX, "%.2f");
  if (ImGui::Button("Save")) {
    try {
      save(outPathBuf);
    } catch (std::filesystem::filesystem_error e) {
      savePopupText = e.what();
      ImGui::OpenPopup("SavePopup");
    }
  }
  if (ImGui::BeginPopup("SavePopup")) {
    ImGui::Text("%s", savePopupText.c_str());
    ImGui::EndPopup();
  }
}

void Editor::save(const std::filesystem::path& path) {
  std::ofstream out(path);
  if (!out) {
    savePopupText = "Could not save level data";
    ImGui::OpenPopup("SavePopup");
    return;
  }

  saveBackground(out);
  auto top_left_corner = saveTilemap(out);

  out.close();
  savePopupText = "Saved successfully";
  ImGui::OpenPopup("SavePopup");
}

void Editor::saveBackground(std::ofstream& out) {
  if (noBg || backgroundPath.empty()) {
    return;
  }
  out << "[Background]\n";
  out << backgroundPath.c_str() << '\n';
  out << backgroundRepeated << " " << backgroundMoving << " "
      << backgroundSize[0] << " " << backgroundSize[1] << '\n';
}

sf::Vector2i Editor::saveTilemap(std::ofstream& out) {
  auto [min_x_it, max_x_it] =
      std::minmax_element(sparseTileMap.begin(), sparseTileMap.end(),
                          [](const auto& a, const auto& b) {
                            return a.first.first < b.first.first;
                          });
  auto [min_y_it, max_y_it] =
      std::minmax_element(sparseTileMap.begin(), sparseTileMap.end(),
                          [](const auto& a, const auto& b) {
                            return a.first.second < b.first.second;
                          });
  auto [min_c_x_it, max_c_x_it] = std::minmax_element(
      sparseTileColliders.begin(), sparseTileColliders.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });
  auto [min_c_y_it, max_c_y_it] = std::minmax_element(
      sparseTileColliders.begin(), sparseTileColliders.end(),
      [](const auto& a, const auto& b) { return a.second < b.second; });
  int min_x =
      std::min(sparseTileMap.empty() ? std::numeric_limits<int>::max()
                                     : min_x_it->first.first,
               sparseTileColliders.empty() ? std::numeric_limits<int>::max()
                                           : min_c_x_it->first);
  int max_x =
      std::max(sparseTileMap.empty() ? std::numeric_limits<int>::min()
                                     : max_x_it->first.first,
               sparseTileColliders.empty() ? std::numeric_limits<int>::min()
                                           : max_c_x_it->first);
  int min_y =
      std::min(sparseTileMap.empty() ? std::numeric_limits<int>::max()
                                     : min_y_it->first.second,
               sparseTileColliders.empty() ? std::numeric_limits<int>::max()
                                           : min_c_y_it->second);
  int max_y =
      std::max(sparseTileMap.empty() ? std::numeric_limits<int>::min()
                                     : max_y_it->first.second,
               sparseTileColliders.empty() ? std::numeric_limits<int>::min()
                                           : max_c_y_it->second);
  int size_x = sparseTileMap.empty() && sparseTileColliders.empty()
                   ? 0
                   : max_x - min_x + 1;
  int size_y = sparseTileMap.empty() && sparseTileColliders.empty()
                   ? 0
                   : max_y - min_y + 1;
  bool no_tileset = !tileset || sparseTileMap.empty();

  out << "[Tilemap]\n";
  out << tileSize << '\n';
  out << (no_tileset ? "<none>" : tilesetPath.c_str()) << '\n';
  if (!no_tileset) {
    out << tileset->tileSize << " ";
  }
  out << size_x << " " << size_y << '\n';
  for (int i = 0, y = min_y; i < size_y; i++, y++) {
    for (int j = 0, x = min_x; j < size_x; j++, x++) {
      if (!no_tileset) {
        auto it = sparseTileMap.find({x, y});
        int tile = it == sparseTileMap.end() ? -1 : it->second.first;
        out << tile << " ";
      }
      out << sparseTileColliders.count({x, y}) << " ";
    }
    out << '\n';
  }

  return {min_x, min_y};
}

void Editor::draw() {
  if (!noBg) window.draw(background);
  window.draw(grid);
  window.draw(sparseGrid);
  for (auto& [coords, tile] : sparseTileMap) {
    window.draw(tile.second);
  }
  for (auto& coords : sparseTileColliders) {
    tileCollider.setPosition(
        {coords.first * gridTileSize.x, coords.second * gridTileSize.y});
    window.draw(tileCollider);
  }
  window.draw(zeroPoint);
  window.draw(tileSelection);
  ImGui::SFML::Render(window);
}
