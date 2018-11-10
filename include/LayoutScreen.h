#ifndef LAYOUTSCREEN_H
#define LAYOUTSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <string>
#include "global.h"

class LayoutScreen
{
  public:
    LayoutScreen();
    int Run(sf::RenderWindow &window);

  private:
  std::array<sf::Texture, 4> chessboard_t;
  std::array<sf::Sprite, 4> chessboardSprite;
  sf::Vector2i mousePosition;
  std::array<sf::RectangleShape, 4> rectangle;
};

#endif // LAYOUTSCREEN_H
