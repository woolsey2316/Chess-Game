#ifndef PROMOTIONSCREEN_H
#define PROMOTIONSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>
#include "global.h"

extern const std::string board_[8][8];

class PromotionScreen
{
  public:
    PromotionScreen(std::string playerTurn);
    std::string Run(sf::RenderWindow &App);
    std::string getPromotionOption();

  protected:

  private:
  sf::Texture texture[6];
  sf::Vector2i mousePosition;
  sf::Sprite chesspieceSprite[6];
  int row, col;
  bool isWhite;
  std::string piece;
};

#endif // PROMOTIONSCREEN_H
