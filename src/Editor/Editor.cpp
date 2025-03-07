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
#include <string>

#include "Core/utility.h"

using namespace std::literals;

Editor::Editor()
    : grid(gridTileSize, {}, gridColor),
      sparseGrid(gridTileSize * sparseGridScale, {}, sparseGridColor) {
  sf::Vector2u screen_size = {1200, 840};
  sf::View view = {{0, 0}, sf::Vector2f{screen_size}};

  currentPosition = view.getCenter();
  window.create(sf::VideoMode{screen_size}, "Level editor");
  window.setFramerateLimit(60);
  if (!ImGui::SFML::Init(window, false)) {
    std::cerr << "failed to init imgui-sfml window" << std::endl;
    window.close();
  }
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("resources/fonts/DroidSansMono.ttf", 13, nullptr,
                               io.Fonts->GetGlyphRangesCyrillic());
  if (!ImGui::SFML::UpdateFontTexture()) {
    std::cerr << "failed to load font" << std::endl;
    window.close();
  }
  setView(view);

  selection.setFillColor(sf::Color::Transparent);
  selection.setOutlineThickness(2.f);
  selection.setSize(gridTileSize);
  setSelect(SelectState::None);

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
    moveUpButtonClicked = false;
    moveDownButtonClicked = false;
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
    case SelectState::None:
      selection.setOutlineColor(noneSelectColor);
      selectionText.setFillColor(noneSelectColor);
      selectionText.setString("None");
      break;
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
      if (selectedEntity) {
        selectionText.setString(selectedEntity->text.getString());
      } else {
        selectionText.setString("None");
      }
      break;
  }
  auto bounds = selectionText.getLocalBounds();
  selectionText.setOrigin(
      {bounds.position.x, bounds.position.y + bounds.size.y + 5.f});
}

bool Editor::isSelected(SelectState state) { return selectState == state; }

void Editor::selectEntity(Entity* entity) {
  if (selectedEntity) {
    selectedEntity->frame.setOutlineColor(entityColor);
    selectedEntity->text.setFillColor(entityColor);
  }
  selectedEntity = entity;
  if (selectedEntity) {
    selectedEntity->frame.setOutlineColor(selectedEntityColor);
    selectedEntity->text.setFillColor(selectedEntityColor);
  }
  setSelect(SelectState::Entity);
}

void Editor::prepareEntity(Entity& entity) {
  auto& frame = entity.frame;
  frame.setSize(gridTileSize);
  frame.setFillColor(sf::Color::Transparent);
  frame.setOutlineThickness(scale * 2.f);
  auto& text = entity.text;
  text.setOrigin(text.getGlobalBounds().getCenter());
  text.move(gridTileSize / 2.f);
  float factor = (gridTileSize.x - 5.f) / text.getLocalBounds().size.x;
  text.setScale({factor, factor});
}

void Editor::setTile(sf::Vector2i position, int tile) {
  auto element =
      std::make_pair(tile, sf::Sprite(*tilesetTexture, tileset.getRect(tile)));
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
  if (auto keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->code == moveUpKey) {
      if (!moveUpButtonPressed) {
        moveUpButtonClicked = true;
      }
      moveUpButtonPressed = true;
    }
    if (keyPressed->code == moveDownKey) {
      if (!moveDownButtonPressed) {
        moveDownButtonClicked = true;
      }
      moveDownButtonPressed = true;
    }
  }
  if (auto keyReleased = event.getIf<sf::Event::KeyReleased>()) {
    if (keyReleased->code == moveUpKey) {
      moveUpButtonPressed = false;
    }
    if (keyReleased->code == moveDownKey) {
      moveDownButtonPressed = false;
    }
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
    toChangeScale = true;
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
    } else if (isSelected(SelectState::Entity)) {
      if (placeTile && selectedEntity) {
        selectedEntity->position = position;
      }
    }
  }
}

void Editor::handleKeys(sf::Event event) {
  auto keyReleased = event.getIf<sf::Event::KeyReleased>();
  if (!keyReleased) return;
  if (keyReleased->code == sf::Keyboard::Key::Q) paintTiles = !paintTiles;
  if (keyReleased->code == sf::Keyboard::Key::Escape) {
    setSelect(SelectState::None);
  }
  if (keyReleased->code == sf::Keyboard::Key::E) {
    setSelect(isSelected(SelectState::Collider) ? SelectState::None
                                                : SelectState::Collider);
  }
  if (!tilesetTexture) return;
  if (keyReleased->code == sf::Keyboard::Key::Tab && selectedTile != -1) {
    int nextTile = selectedTile + (keyReleased->shift ? -1 : 1);
    selectedTile =
        std::min<int>(std::max(nextTile, 0), tileset.getRectAmount());
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

  ImGui::Begin("Level editor");
  widgetWindowRect.position = ImGui::GetWindowPos();
  widgetWindowRect.size = ImGui::GetWindowSize();
  ImGui::Checkbox("Paint tiles (Q)", &paintTiles);
  if (CustomSelectable("None (Esc)", isSelected(SelectState::None),
                       ImGuiCol_MenuBarBg)) {
    setSelect(SelectState::None);
  }
  if (CustomSelectable("Collider (E)", isSelected(SelectState::Collider),
                       ImGuiCol_MenuBarBg)) {
    setSelect(isSelected(SelectState::Collider) ? SelectState::None
                                                : SelectState::Collider);
  }

  ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
  if (ImGui::BeginTabBar("ResourcesTabBar", tab_bar_flags)) {
    if (ImGui::BeginTabItem("Background")) {
      backgroundWidget();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Tiles")) {
      tilesetLoadWidget();
      tilesetSelectWidget();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Entities")) {
      entitiesWidget();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Sprite sheets")) {
      spriteSheetsWidget();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Actions")) {
      actionsWidget();
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
      loadTilesetFile();
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
  if (!tilesetTexture) return;
  auto [w, h] = tileset.getSize();

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
      selectableTileSprite->setTextureRect(tileset.getRect(idx));
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

void Editor::entitiesWidget() {
  ImGui::AlignTextToFramePadding();
  ImGui::Text("Add:");
  ImGui::SameLine();
  Entity* addedEntity = nullptr;
  if (ImGui::Button("Player")) {
    addedEntity = &entities.emplace_back(Entity{font, Entity::Player{}});
  }
  ImGui::SameLine();
  if (ImGui::Button("Character")) {
    addedEntity = &entities.emplace_back(Entity{font, Entity::Character{}});
  }
  ImGui::SameLine();
  if (ImGui::Button("Prop")) {
    addedEntity = &entities.emplace_back(Entity{font, Entity::Prop{}});
  }
  ImGui::SameLine();
  if (ImGui::Button("Trigger")) {
    addedEntity = &entities.emplace_back(Entity{font, Entity::Trigger{}});
  }
  if (addedEntity) {
    prepareEntity(*addedEntity);
    selectEntity(addedEntity);
  }

  ImGui::SetNextWindowSizeConstraints(ImVec2(0.f, 0.f), ImVec2(FLT_MAX, 500.f));
  ImGui::BeginChild("Entities", {0, 0},
                    ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

  const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
  auto to_delete = entities.end();
  for (auto it = entities.begin(); it != entities.end(); ++it) {
    auto& entity = *it;
    auto name = std::visit(
        overloaded{[](Entity::Player& player) { return "Player"; },
                   [](Entity::Character& character) { return "Character"; },
                   [](Entity::Prop& prop) { return "Prop"; },
                   [](Entity::Trigger& trigger) { return "Trigger"; }},
        entity.data);
    ImGuiTreeNodeFlags node_flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap;
    if (isSelected(SelectState::Entity) && selectedEntity == &entity)
      node_flags |= ImGuiTreeNodeFlags_Selected;
    ImGui::PushID(&entity);
    bool node_open =
        ImGui::TreeNodeEx("entity", node_flags, "%s @ %p", name, &entity);
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
      selectEntity(&entity);
    ImGui::SameLine(ImGui::GetWindowWidth() - 63, spacing);
    if (ImGui::SmallButton("Delete")) {
      to_delete = it;
    }
    if (node_open) {
      ImGui::Text("Position: %d,%d", entity.position.x, entity.position.y);
      ImGui::TreePop();
    }
    ImGui::PopID();
  }
  if (to_delete != entities.end()) {
    auto& entity = *to_delete;
    auto action_holder = std::visit(
        overloaded{[](Entity::Player& player) -> Action** { return nullptr; },
                   [](Entity::Character& character) {
                     return &character.interactible.action;
                   },
                   [](Entity::Prop& prop) { return &prop.interactible.action; },
                   [](Entity::Trigger& trigger) {
                     return &trigger.interactible.action;
                   }},
        entity.data);
    if (action_holder) {
      actionHolders[*action_holder].erase(action_holder);
      if (auto it = holderHolders.find(action_holder);
          it != holderHolders.end()) {
        for (Action* action : it->second) {
          action->holderBuf = nullptr;
          action->holderEntityBuf = nullptr;
        }
        holderHolders.erase(it);
      }
    }
    if (isSelected(SelectState::Entity) && &entity == selectedEntity)
      selectEntity(nullptr);
    entities.erase(to_delete);
  }

  ImGui::EndChild();
}

void Editor::spriteSheetsWidget() { ImGui::Text("sprite sheets"); }

void Editor::actionsWidget() {
  currentHighlightedAction = nullptr;

  ImGui::AlignTextToFramePadding();
  ImGui::Text("Level start:");
  ImGui::SameLine();
  actionTarget(startAction);

  ImGui::SeparatorText("Interactibles");
  for (auto& entity : entities) {
    Entity::Interactible* interactibleAddr = nullptr;
    const char* name;
    std::visit(overloaded{[](Entity::Player& player) {},
                          [&](Entity::Character& character) {
                            name = "Character";
                            interactibleAddr = &character.interactible;
                          },
                          [&](Entity::Prop& prop) {
                            name = "Prop";
                            interactibleAddr = &prop.interactible;
                          },
                          [&](Entity::Trigger& trigger) {
                            name = "Trigger";
                            interactibleAddr = &trigger.interactible;
                          }},
               entity.data);
    if (!interactibleAddr) continue;
    ImGui::PushID(&entity);
    std::stringstream ss;
    ss << name << " @ " << &entity;
    ImGui::SetNextItemAllowOverlap();
    ImGui::AlignTextToFramePadding();
    if (ImGui::Selectable(
            ss.str().c_str(),
            isSelected(SelectState::Entity) && selectedEntity == &entity,
            highlightedHolderEntity == &entity ? ImGuiSelectableFlags_Highlight
                                               : ImGuiSelectableFlags_None)) {
      selectEntity(&entity);
    }

    if (ImGui::BeginDragDropSource()) {
      std::pair<Action**, Entity*> payload = {&interactibleAddr->action,
                                              &entity};
      ImGui::SetDragDropPayload("HolderPayload",
                                reinterpret_cast<const void*>(&payload),
                                sizeof(decltype(payload)));
      ImGui::Text("%s @ %p", name, &entity);
      ImGui::EndDragDropSource();
    }

    ImGui::SameLine();
    actionTarget(interactibleAddr->action);
    ImGui::PopID();
  }
  highlightedHolderEntity = nullptr;

  ImGui::SeparatorText("Actions");
  auto to_delete = actions.end();
  auto to_swap = actions.end();
  for (auto it = actions.begin(); it != actions.end(); ++it) {
    auto& action = *it;
    ImGui::PushID(&action);
    ImGuiTreeNodeFlags node_flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap;

    if (highlightedAction == &action) node_flags |= ImGuiTreeNodeFlags_Framed;
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNodeEx("##node", node_flags);
    auto preview_value =
        std::visit(overloaded{[](ActionData::Text&) { return "Text"; },
                              [](ActionData::Image&) { return "Image"; },
                              [](ActionData::Sound&) { return "Sound"; },
                              [](ActionData::Music&) { return "Music"; },
                              [](ActionData::Level&) { return "Level"; },
                              [](ActionData::Action&) { return "Action"; },
                              [](auto&) { return "Something else"; }},
                   action.data.data);
    if (ImGui::BeginDragDropSource()) {
      Action* addr = &action;
      ImGui::SetDragDropPayload("ActionPayload", &addr, sizeof(Action*));
      ImGui::Text("%s @ %p", preview_value, &action);
      ImGui::EndDragDropSource();
    }
    if (ImGui::IsItemHovered()) {
      if (moveUpButtonClicked && it != actions.begin()) {
        to_swap = std::prev(it);
      } else if (moveDownButtonClicked && std::next(it) != actions.end()) {
        to_swap = it;
      }
    }
    ImGui::SameLine(0.0f, 0.0f);
    if (ImGui::Button(preview_value)) ImGui::OpenPopup("ActionTypePopup");
    if (ImGui::BeginPopup("ActionTypePopup")) {
      bool selected = false;
      if (ImGui::Selectable("Text", std::holds_alternative<ActionData::Text>(
                                        action.data.data))) {
        action.data.data = ActionData::Text{};
        selected = true;
      }
      if (ImGui::Selectable("Image", std::holds_alternative<ActionData::Image>(
                                         action.data.data))) {
        action.data.data = ActionData::Image{};
        selected = true;
      }
      if (ImGui::Selectable("Sound", std::holds_alternative<ActionData::Sound>(
                                         action.data.data))) {
        action.data.data = ActionData::Sound{};
        selected = true;
      }
      if (ImGui::Selectable("Music", std::holds_alternative<ActionData::Music>(
                                         action.data.data))) {
        action.data.data = ActionData::Music{};
        selected = true;
      }
      if (ImGui::Selectable("Level", std::holds_alternative<ActionData::Level>(
                                         action.data.data))) {
        action.data.data = ActionData::Level{};
        selected = true;
      }
      if (ImGui::Selectable(
              "Action",
              std::holds_alternative<ActionData::Action>(action.data.data))) {
        action.data.data = ActionData::Action{};
        selected = true;
      }
      if (selected) {
        action.pathBuf.clear();
        action.intBuf.clear();
        action.actionBuf = nullptr;
        action.holderBuf = nullptr;
        action.holderEntityBuf = nullptr;
      }
      ImGui::EndPopup();
    }
    ImGui::SameLine();
    ImGui::Text("@ %p", &action);
    ImGui::SameLine();
    actionTarget(action.next);
    ImGui::SameLine(ImGui::GetWindowWidth() - 63);
    if (ImGui::SmallButton("Delete")) {
      to_delete = it;
    }
    if (node_open) {
      std::visit(
          overloaded{
              [](ActionData::Text& text) {
                ImGui::InputTextMultiline("Text##field", &text.text);
              },
              [&](ActionData::Image& image) {
                ImGui::InputText("Path", &action.pathBuf);
              },
              [&](ActionData::Sound& sound) {
                ImGui::InputText("Path", &action.pathBuf);
                ImGui::Checkbox("Looping", &sound.looping);
                ImGui::DragFloat("Offset (s)", &sound.offsetSeconds, 0.05f, 0.f,
                                 FLT_MAX, "%.2f");
                ImGui::DragFloat("Volume", &sound.volume, 0.5f, 0.f, 100.f,
                                 "%.1f");
              },
              [&](ActionData::Music& music) {
                ImGui::InputText("Path", &action.pathBuf);
                ImGui::Checkbox("Looping", &music.looping);
                ImGui::DragFloat("Offset (s)", &music.offsetSeconds, 0.05f, 0.f,
                                 FLT_MAX, "%.2f");
                ImGui::DragFloat("Volume", &music.volume, 0.5f, 0.f, 100.f,
                                 "%.1f");
              },
              [&](ActionData::Level& level) {
                ImGui::InputText("Path", &action.pathBuf);
                ImGui::InputText("Player spot", &action.intBuf,
                                 ImGuiInputTextFlags_CharsDecimal);
              },
              [&](ActionData::Action&) {
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Holder:");
                ImGui::SameLine();
                holderTarget(action, "##holder");
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Action:");
                ImGui::SameLine();
                actionTarget(action.actionBuf, "##action");
              },
              [](auto&) {  // TODO(des): finish fields for all other types
                ImGui::Text("nothing yet...");
              }},
          action.data.data);
      ImGui::TreePop();
    }
    ImGui::PopID();
  }
  if (to_swap != actions.end()) {
    actions.splice(to_swap, actions, std::next(to_swap));
  } else if (to_delete != actions.end()) {
    auto& action = *to_delete;
    if (action.next) {
      actionHolders[action.next].erase(&action.next);
    }
    if (action.holderBuf) {
      holderHolders[action.holderBuf].erase(&action);
    }
    if (auto it = actionHolders.find(&action); it != actionHolders.end()) {
      for (Action** holder : it->second) {
        *holder = nullptr;
      }
      actionHolders.erase(it);
    }
    actions.erase(to_delete);
  }

  highlightedAction = currentHighlightedAction;

  if (ImGui::Button("Add")) ImGui::OpenPopup("AddActionPopup");
  if (ImGui::BeginPopup("AddActionPopup")) {
    if (ImGui::Selectable("Text")) {
      actions.emplace_back(Action{ActionData::Text{}});
    }
    if (ImGui::Selectable("Image")) {
      actions.emplace_back(Action{ActionData::Image{}});
    }
    if (ImGui::Selectable("Sound")) {
      actions.emplace_back(Action{ActionData::Sound{}});
    }
    if (ImGui::Selectable("Music")) {
      actions.emplace_back(Action{ActionData::Music{}});
    }
    if (ImGui::Selectable("Level")) {
      actions.emplace_back(Action{ActionData::Level{}});
    }
    if (ImGui::Selectable("Action")) {
      actions.emplace_back(Action{ActionData::Action{}});
    }
    ImGui::EndPopup();
  }
}

void Editor::actionTarget(Action*& action, const char* descriptor) {
  std::stringstream ss;
  if (action) {
    ss << action;
  } else {
    ss << "<no action>";
  }
  ss << descriptor;
  if (ImGui::Button(ss.str().c_str())) {
    action = nullptr;
    actionHolders[action].erase(&action);
  }
  if (ImGui::IsItemHovered()) {
    currentHighlightedAction = action;
  }
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("ActionPayload")) {
      action = *(Action**)payload->Data;
      actionHolders[action].insert(&action);
    }
    ImGui::EndDragDropTarget();
  }
}

void Editor::holderTarget(Action& target, const char* descriptor) {
  std::stringstream ss;
  if (target.holderEntityBuf) {
    ss << target.holderEntityBuf;
  } else {
    ss << "<no holder>";
  }
  ss << descriptor;
  if (ImGui::Button(ss.str().c_str())) {
    target.holderBuf = nullptr;
    target.holderEntityBuf = nullptr;
    holderHolders[target.holderBuf].erase(&target);
  }
  if (ImGui::IsItemHovered()) {
    highlightedHolderEntity = target.holderEntityBuf;
  }
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("HolderPayload")) {
      auto data =
          *reinterpret_cast<std::pair<Action**, Entity*>*>(payload->Data);
      target.holderBuf = data.first;
      target.holderEntityBuf = data.second;
      holderHolders[target.holderBuf].insert(&target);
    }
    ImGui::EndDragDropTarget();
  }
}

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
  auto rollback =
      [=, &backref = *this, backgroundTexture = std::move(backgroundTexture),
       backgroundSize = sf::Vector2f(backgroundSize[0], backgroundSize[1]),
       tilesetTexture = std::move(tilesetTexture), tiles = std::move(tiles),
       colliders = std::move(colliders), entities = std::move(entities),
       actions = std::move(actions), actionHolders = std::move(actionHolders),
       holderHolders = std::move(holderHolders)]() mutable {
        // background inner state
        if (backgroundTexture) {
          backgroundTexture->setRepeated(backgroundRepeated);
          backref.background.setTexture(backgroundTexture.get(),
                                        backgroundSize);
        } else {
          backref.background.unsetTexture();
        }
        backref.background.setMoving(backgroundMoving);
        backref.background.setView(backref.window.getView());
        backref.backgroundTexture = std::move(backgroundTexture);
        backref.backgroundTexturePath = backgroundTexturePath;
        // background editor state
        backref.noBackground = noBackground;
        backref.backgroundPathBuf = backgroundPathBuf;
        backref.backgroundSize[0] = backgroundSize.x;
        backref.backgroundSize[1] = backgroundSize.y;
        backref.backgroundMoving = backgroundMoving;
        backref.backgroundRepeated = backgroundRepeated;
        // tilemap inner state
        backref.tiles = std::move(tiles);
        backref.colliders = std::move(colliders);
        backref.tilesetTexture = std::move(tilesetTexture);
        backref.tilesetPath = tilesetPath;
        backref.selectedTile = selectedTile;
        // tilemap editor state
        backref.tilesetTileSize = tilesetTileSize;
        backref.tilesetPathBuf = tilesetPathBuf;
        // entities & actions inner state
        backref.entities = std::move(entities);
        backref.actions = std::move(actions);
        backref.actionHolders = std::move(actionHolders);
        backref.holderHolders = std::move(holderHolders);
        backref.startAction = startAction;
      };
  try {
    std::ifstream in(filename);
    LevelData data = LevelData::load(in);
    loadBackground(data.background);
    loadTilemap(data.tilemap);
    loadEntities(data.entities);
    loadActions(data.actions);
    bindActionRefs(data);
    loadPopupText = "Loaded successfully";
  } catch (const std::exception& e) {
    loadPopupText =
        "failed to load level data from " + filename.string() + ": " + e.what();
    rollback();
  }
  ImGui::OpenPopup("LoadPopup");
}

void Editor::save(const std::filesystem::path& filename) {
  std::ofstream out(filename);
  try {
    LevelData data;
    saveBackground(data);
    sf::Vector2i origin = saveTilemap(data);
    saveEntities(data.entities, origin);
    saveActions(data.actions);
    bindActionIndices(data);
    data.save(out);
    savePopupText = "Saved successfully";
  } catch (const std::runtime_error& e) {
    savePopupText =
        "failed to save level data to " + filename.string() + ": " + e.what();
  }
  ImGui::OpenPopup("SavePopup");
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

void Editor::loadBackground(std::optional<LevelData::Background>& data) {
  if (!data) {
    background.unsetTexture();
    backgroundTexture.reset();
    backgroundTexturePath.clear();
    return;
  }
  noBackground = false;
  backgroundPathBuf = data->texturePath;
  backgroundSize[0] = data->size.x;
  backgroundSize[1] = data->size.y;
  backgroundMoving = data->moving;
  backgroundRepeated = data->repeated;
  loadBackgroundFile();
}

void Editor::saveBackground(LevelData& data) {
  if (noBackground || backgroundTexturePath.empty()) return;
  data.background.emplace();
  data.background->texturePath = backgroundTexturePath;
  data.background->repeated = backgroundRepeated;
  data.background->moving = backgroundMoving;
  data.background->size = {backgroundSize[0], backgroundSize[1]};
}

void Editor::loadTilesetFile() {
  tilesetTexture = std::make_unique<sf::Texture>(tilesetPathBuf);
  tileset.setTexture(*tilesetTexture);
  tileset.tileSize.x = tileset.tileSize.y = tilesetTileSize;
  tilesetPath = tilesetPathBuf;
  selectedTile = -1;
  tiles.clear();
  if (selectableTileSprite) {
    selectableTileSprite->setTexture(*tilesetTexture);
  } else {
    selectableTileSprite = std::make_unique<sf::Sprite>(*tilesetTexture);
  }
}

void Editor::loadTilemap(LevelData::Tilemap& data) {
  if (data.noTileset) {
    tilesetTexture.reset();
    tilesetPath.clear();
    selectedTile = -1;
    tiles.clear();
  } else {
    tilesetTileSize = data.tilesetTileSize;
    tilesetPathBuf = data.tilesetPath;
    loadTilesetFile();
  }
  colliders.clear();
  for (int i = 0; i < data.height; i++) {
    for (int j = 0; j < data.width; j++) {
      if (!data.noTileset) setTile({j, i}, data.tiles[i * data.width + j]);
      if (data.colliders[i * data.width + j]) setCollider({j, i});
    }
  }
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
  data.tilemap.noTileset = !tilesetTexture || tiles.empty();
  data.tilemap.tileSize = saveTileSize;
  if (!data.tilemap.noTileset) {
    data.tilemap.tilesetPath = tilesetPath;
    data.tilemap.tilesetTileSize =
        tileset.tileSize
            .x;  // NOTE(des): should non-square tiles be really supported?
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
  if (width == 0 && height == 0) return {0, 0};
  return {min_x, min_y};
}

void Editor::loadEntities(std::vector<EntityData>& data) {
  entities.clear();
  selectedEntity = nullptr;
  for (auto& entity : data) {
    std::visit(
        overloaded{[&](EntityData::Player& player) {
                     entities.emplace_back(Entity{font, Entity::Player{}});
                   },
                   [&](EntityData::Character& character) {
                     entities.emplace_back(Entity{font, Entity::Character{}});
                     // TODO(des): drawable data
                   },
                   [&](EntityData::Prop& prop) {
                     entities.emplace_back(Entity{font, Entity::Prop{}});
                     // TODO(des): drawable data
                   },
                   [&](EntityData::Trigger& trigger) {
                     entities.emplace_back(Entity{font, Entity::Trigger{}});
                   }},
        entity.data);
    entities.back().position = entity.position;
    prepareEntity(entities.back());
    selectEntity(&entities.back());  // NOTE(des): this hack stinks
  }
  setSelect(SelectState::None);
}

void Editor::saveEntities(std::vector<EntityData>& data, sf::Vector2i origin) {
  for (auto& entity : entities) {
    std::visit(
        overloaded{[&](Entity::Player& player) {
                     data.emplace_back(EntityData{entity.position - origin,
                                                  EntityData::Player{}});
                   },
                   [&](Entity::Character& character) {
                     data.emplace_back(EntityData{entity.position - origin,
                                                  EntityData::Character{}});
                   },
                   [&](Entity::Prop& prop) {
                     data.emplace_back(EntityData{entity.position - origin,
                                                  EntityData::Prop{}});
                   },
                   [&](Entity::Trigger& trigger) {
                     data.emplace_back(EntityData{entity.position - origin,
                                                  EntityData::Trigger{}});
                   }},
        entity.data);
  }
}

void Editor::loadActions(std::vector<ActionData>& data) {
  actions.clear();
  actionHolders.clear();
  holderHolders.clear();
  highlightedAction = nullptr;
  highlightedHolderEntity = nullptr;
  for (auto& actionData : data) {
    auto& action = actions.emplace_back(Action{actionData});
    std::visit(
        [&](auto& actionType) {
          using ActionType = std::decay_t<decltype(actionType)>;
          if constexpr (is_one_of_v<ActionType, ActionData::TextFont,
                                    ActionData::TextSound,
                                    ActionData::CharNameFont,
                                    ActionData::CharIcon, ActionData::Image,
                                    ActionData::Sound, ActionData::Music,
                                    ActionData::Level>) {
            action.pathBuf = actionType.filename;
          }
          if constexpr (is_one_of_v<ActionType, ActionData::Level>) {
            action.intBuf = std::to_string(actionType.playerSpot);
          }
        },
        actionData.data);
  }
}

void Editor::saveActions(std::vector<ActionData>& data) {
  for (auto& action : actions) {
    std::visit(
        [&](auto& actionType) {
          using ActionType = std::decay_t<decltype(actionType)>;
          if constexpr (is_one_of_v<ActionType, ActionData::TextFont,
                                    ActionData::TextSound,
                                    ActionData::CharNameFont,
                                    ActionData::CharIcon, ActionData::Image,
                                    ActionData::Sound, ActionData::Music,
                                    ActionData::Level>) {
            actionType.filename = action.pathBuf;
          }
          if constexpr (is_one_of_v<ActionType, ActionData::Level>) {
            actionType.playerSpot = std::stoi(action.intBuf);
          }
        },
        action.data.data);
    data.emplace_back(action.data);
  }
}

void Editor::bindActionRefs(LevelData& data) {
  std::vector<Action*> actionRefs(actions.size());
  std::vector<Action**> holderRefs;
  std::vector<Entity*> holderEntityRefs;
  {
    auto it = actions.begin();
    for (std::size_t i = 0; i < actions.size(); ++it, i++) {
      actionRefs[i] = &*it;
    }
  }

  {
    auto it = entities.begin();
    for (std::size_t i = 0; i < data.entities.size(); ++it, i++) {
      auto& entityData = data.entities[i];
      auto& entity = *it;
      Action** holder = nullptr;
      Entity* holderEntity = nullptr;
      std::visit(
          overloaded{
              [&](EntityData::Player& player) {},
              [&](EntityData::Character& character) {
                Action*& action = std::get<Entity::Character>(entity.data)
                                      .interactible.action;
                if (character.action != -1) {
                  action = actionRefs.at(character.action);
                  actionHolders[action].insert(&action);
                }
                holder = &action;
                holderEntity = &entity;
              },
              [&](EntityData::Prop& prop) {
                Action*& action =
                    std::get<Entity::Prop>(entity.data).interactible.action;
                if (prop.action != -1) {
                  action = actionRefs.at(prop.action);
                  actionHolders[action].insert(&action);
                }
                holder = &action;
                holderEntity = &entity;
              },
              [&](EntityData::Trigger& trigger) {
                Action*& action =
                    std::get<Entity::Trigger>(entity.data).interactible.action;
                if (trigger.action != -1) {
                  action = actionRefs.at(trigger.action);
                  actionHolders[action].insert(&action);
                }
                holder = &action;
                holderEntity = &entity;
              }},
          entityData.data);
      holderRefs.push_back(holder);
      holderEntityRefs.push_back(holderEntity);
    }
  }

  auto it = actions.begin();
  for (std::size_t i = 0; i < data.nextAction.size(); ++it, i++) {
    int next = data.nextAction[i];
    auto& action = *it;
    if (next != -1) {
      action.next = actionRefs.at(next);
      actionHolders[action.next].insert(&action.next);
    }
    if (auto actionAction = std::get_if<ActionData::Action>(&it->data.data)) {
      if (actionAction->holder != -1) {
        it->holderBuf = holderRefs.at(actionAction->holder);
        it->holderEntityBuf = holderEntityRefs.at(actionAction->holder);
        holderHolders[it->holderBuf].insert(&*it);
      }
      if (actionAction->action != -1) {
        it->actionBuf = actionRefs.at(actionAction->action);
        actionHolders[it->actionBuf].insert(&it->actionBuf);
      }
    }
  }

  if (data.meta.startAction != -1) {
    startAction = actionRefs.at(data.meta.startAction);
    actionHolders[startAction].insert(&startAction);
  } else {
    startAction = nullptr;
  }
}

void Editor::bindActionIndices(LevelData& data) {
  std::unordered_map<Action*, int> actionIndex;
  std::unordered_map<Action**, int> holderIndex;
  actionIndex[nullptr] = -1;
  holderIndex[nullptr] = -1;
  {
    int i = 0;
    for (auto& action : actions) {
      actionIndex[&action] = i++;
    }
  }

  {
    auto it = entities.begin();
    for (std::size_t i = 0; i < data.entities.size(); ++it, i++) {
      auto& entityData = data.entities[i];
      auto& entity = *it;
      std::visit(
          overloaded{
              [&](EntityData::Player& player) {},
              [&](EntityData::Character& character) {
                auto& holder = std::get<Entity::Character>(entity.data)
                                   .interactible.action;
                character.action = actionIndex.at(holder);
                holderIndex[&holder] = i;
              },
              [&](EntityData::Prop& prop) {
                auto& holder =
                    std::get<Entity::Prop>(entity.data).interactible.action;
                prop.action = actionIndex.at(holder);
                holderIndex[&holder] = i;
              },
              [&](EntityData::Trigger& trigger) {
                auto& holder =
                    std::get<Entity::Trigger>(entity.data).interactible.action;
                trigger.action = actionIndex.at(holder);
                holderIndex[&holder] = i;
              }},
          entityData.data);
    }
  }

  data.nextAction.resize(data.actions.size());
  auto it = actions.begin();
  for (std::size_t i = 0; i < data.nextAction.size(); ++it, i++) {
    auto& action = *it;
    data.nextAction[i] = actionIndex.at(action.next);
    if (auto actionAction =
            std::get_if<ActionData::Action>(&data.actions[i].data)) {
      if (action.actionBuf) {
        actionAction->action = actionIndex.at(action.actionBuf);
        actionAction->holder = holderIndex.at(action.holderBuf);
      }
    }
  }

  data.meta.startAction = actionIndex.at(startAction);
}

void Editor::draw() {
  if (!noBackground) window.draw(background);
  window.draw(grid);
  window.draw(sparseGrid);
  for (auto& [coords, tile] : tiles) {
    window.draw(tile.second);
  }
  for (auto& entity : entities) {
    if (toChangeScale) {
      entity.frame.setOutlineThickness(scale * 2.f);
    }
    sf::RenderStates states;
    states.transform.translate(
        gridTileSize.componentWiseMul(sf::Vector2f{entity.position}));
    window.draw(entity.frame, states);
    window.draw(entity.text, states);
  }
  for (auto& coords : colliders) {
    colliderFrame.setPosition(
        {coords.first * gridTileSize.x, coords.second * gridTileSize.y});
    window.draw(colliderFrame);
  }
  if (toChangeScale) {
    selection.setOutlineThickness(scale * 2.f);
    colliderFrame.setOutlineThickness(scale * 2.f);
    selectionText.setScale({scale, scale});
    zeroPoint.setRadius(scale * 3.f);
    zeroPoint.setOrigin(zeroPoint.getGeometricCenter());
    toChangeScale = false;
  }
  window.draw(zeroPoint);
  window.draw(selection);
  window.draw(selectionText);
  ImGui::SFML::Render(window);
}
