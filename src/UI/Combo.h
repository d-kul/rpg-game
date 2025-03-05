#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Core/Signal.h"
#include "UI/Element.h"
#include "UI/Frame.h"

class Combo : public UIElement {
 private:
  using sig_t = Signal<int>;

 public:
  class Popup : public UIElement {
    // TODO(des): culling & scrolling if too many options
   private:
    Popup(const sf::Font& font, sf::Vector2f optionSize, unsigned int textSize,
          std::vector<sf::String>& options, sig_t& onSelect_sig,
          int maxOptions);

   public:
    bool handleEvent(sf::Event event) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   public:
    const UIElement::children_t* options;

   private:
    int maxOptions;
    int optionsSize;
    float step;
    float scrollbarStep;
    int start = 0;
    sf::FloatRect rect;
    Frame* optionsFrame;
    Frame* scrollbar;
    friend class Combo;
  };

 public:
  // Constructors, destructor
  Combo(const sf::Font& font, sf::Vector2f size, unsigned int textSize = 30,
        std::vector<sf::String> options = {}, int selectedOptionIdx = -1,
        int maxOptions = 3);

  // Signals
  sig_t& onSelect();

  // Functionality
  bool handleEvent(sf::Event event) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  // Members
  std::vector<sf::String> options;
  int selectedOptionIdx;
  Popup popup;
  bool popupOpened = false;
  sig_t onSelect_sig;
};
