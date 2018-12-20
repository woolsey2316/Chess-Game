#ifndef DRILLSCREEN_H
#define DRILLSCREEN_H

#include "ChessBoardScreen.h"
#include "MoveType.h"
#include "ChessBoard.h"
#include "Connector.h"
#include "Coordinates.h"
#include "ChessGame.h"
#include "main.h"
#include "cScreen.h"

extern const std::string board_[8][8];

class DrillScreen : public ChessBoardScreen {
 public:
  DrillScreen(void);
	int Run(sf::RenderWindow &App);
  void refreshDisplay(sf::RenderWindow &App);
  void loadScreen();

 private:
  sf::Texture sidePanel_t;
  sf::Sprite sidePanel;
  sf::Sprite backButton;
  sf::Texture backButton_t;
  sf::Sprite forwardButton;
  sf::Texture forwardButton_t;
  std::array<sf::RectangleShape, 2> rectangle;
  int currentDrill = 0;

  std::array<sf::Text, 8> drillType;
};

#endif // DRILLSCREEN_H
