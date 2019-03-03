#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <string>
#include "global.h"

class OptionMenu
{
 public:
  OptionMenu(int);
  int Run(sf::RenderWindow &window);
  int getChosenDifficulty();
  void setChosenDifficulty(int diff);

 private:
  std::array<sf::Text, 10> number;
  std::array<sf::CircleShape, 10> circles;
  sf::Vector2i mousePosition;
  sf::Texture background_t;
  sf::Sprite background;
  bool buttonPressed = false;
  int difficulty = 1;

};


#endif
