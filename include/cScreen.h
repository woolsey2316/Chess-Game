#ifndef CSCREEN_H
#define CSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class cScreen
{
 public :
  virtual int Run (sf::RenderWindow &App) = 0;
};

#endif // CSCREEN_H
