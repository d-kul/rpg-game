#include "Frame.h"

Frame::Frame() {}

bool Frame::handleEvent(sf::Event event) { return handleChildrenEvent(event); }

void Frame::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
  drawChildren(target, states);
}
