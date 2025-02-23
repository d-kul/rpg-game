#include "Frame.h"

Frame::Frame() {}

void Frame::handleEvent(sf::Event event) {
  handleChildrenEvent(event);
}

void Frame::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
  drawChildren(target, states);
}
