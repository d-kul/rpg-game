#include "Combo.h"

#include "Core/Logger.h"
#include "UI/Button.h"

using Popup = Combo::Popup;

Popup::Popup(const sf::Font& font, sf::Vector2f optionSize,
             unsigned int textSize, std::vector<sf::String>& options,
             sig_t& onSelect_sig, int maxOptions)
    : maxOptions(maxOptions),
      optionsSize(options.size()),
      step(optionSize.y),
      rect({0.f, 0.f}, {optionSize.x, optionSize.y * maxOptions}) {
  assert(maxOptions <=
         options.size());  // NOTE(des): man, i don't want to deal with this

  {
    auto scrollbar = std::make_unique<Frame>();
    this->scrollbar = scrollbar.get();
    float padding = 5.f;
    float width = 10.f;
    scrollbar->shape.setSize(
        {width, (rect.size.y - 2 * padding) * maxOptions / options.size()});
    scrollbar->shape.setFillColor(sf::Color(0, 0, 0, 100));
    scrollbar->setPosition({rect.size.x - padding - width, padding});
    scrollbarStep = (rect.size.y - 2 * padding) / options.size();
    addChild(std::move(scrollbar));
  }
  {
    auto optionsFrame = std::make_unique<Frame>();
    this->optionsFrame = optionsFrame.get();
    this->options = &optionsFrame->getChildren();
    for (std::size_t i = 0; i < options.size(); i++) {
      auto button =
          std::make_unique<Button>(font, optionSize, options[i], textSize);
      button->move({0, static_cast<float>(optionSize.y * i)});
      button->onClick().subscribe([&, i]() { onSelect_sig(i); });
      optionsFrame->addChild(std::move(button));
    }
    addChild(std::move(optionsFrame));
  }
}

bool Popup::handleEvent(sf::Event event) {
  if (auto mouseWheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>();
      mouseWheelScrolled && pointInside(rect, mouseWheelScrolled->position)) {
    if (start > 0 && mouseWheelScrolled->delta > 0) {
      start--;
      optionsFrame->move({0.f, step});
      scrollbar->move({0.f, -scrollbarStep});
    } else if (start + maxOptions < optionsSize &&
               mouseWheelScrolled->delta < 0) {
      start++;
      optionsFrame->move({0.f, -step});
      scrollbar->move({0.f, scrollbarStep});
    }
  }
  for (std::size_t i = 0; i < maxOptions; i++) {
    auto& child = (*options)[start + i];
    bool res = child->handleEvent(event);
    if (res) return res;
  }
  return false;
}

void Popup::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  {
    auto optionsStates = states;
    optionsStates.transform *= optionsFrame->getTransform();
    for (std::size_t i = 0; i < maxOptions; i++) {
      auto& child = (*options)[start + i];
      target.draw(*child, optionsStates);
    }
  }
  target.draw(*scrollbar, states);
}

Combo::Combo(const sf::Font& font, sf::Vector2f size, unsigned int textSize,
             std::vector<sf::String> options, int selectedOptionIdx,
             int maxOptions)
    : options{std::move(options)},
      selectedOptionIdx{selectedOptionIdx},
      popup{font, size, textSize, this->options, onSelect_sig, maxOptions} {
  auto button = std::make_unique<Button>(
      font, size, selectedOptionIdx < 0 ? "" : options[selectedOptionIdx],
      textSize);
  auto& button_text = button->text;
  button->onClick().subscribe([&]() { popupOpened = !popupOpened; });
  addChild(std::move(button));

  popup.move({0, size.y});
  popup.parent = this;
  onSelect_sig.subscribe([&](int optionIdx) {
    button_text.setString(optionIdx < 0 ? "" : this->options[optionIdx]);
    button_text.setOrigin(button_text.getLocalBounds().getCenter());
    popupOpened = false;
  });
}

Combo::sig_t& Combo::onSelect() { return onSelect_sig; }

bool Combo::handleEvent(sf::Event event) {
  if (popupOpened && popup.handleEvent(event)) return true;
  return handleChildrenEvent(event);
}

void Combo::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  drawChildren(target, states);
  if (popupOpened) target.draw(popup, states);
}
