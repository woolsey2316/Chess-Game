#ifndef ANALYSISSCREEN_H
#define ANALYSISSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "MoveType.h"
#include "ChessBoard.h"
#include "ChessBoardScreen.h"
#include "Connector.h"
#include "Coordinates.h"
#include "ChessGame.h"
#include "main.h"
#include "cScreen.h"

extern const std::string board_[8][8];

class AnalysisScreen : public ChessBoardScreen {
 public:
  AnalysisScreen(void);
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

};

#endif // ANALYSISSCREEN_H
