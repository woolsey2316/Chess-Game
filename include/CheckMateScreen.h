#ifndef CHECKMATESCREEN_H
#define CHECKMATESCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>
#include "global.h"

class CheckMateScreen
{
  public:
    CheckMateScreen();
    int Run(sf::RenderWindow &window);

  private:
  sf::Texture texture;
  sf::Vector2i mousePosition;
  sf::Sprite sprite;
  int row, col;
  bool isWhite;
};


#endif // CHECKMATESCREEN_H
