#ifndef GAMESCREEN_H
#define GAMESCREEN_H

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

class GameScreen : public ChessBoardScreen {
 public:
  GameScreen(void);
	virtual int Run(sf::RenderWindow &App);
  void refreshDisplay(sf::RenderWindow &App);
  void loadScreen();

 private:
  sf::Texture sidePanel_t;
  sf::Sprite sidePanel;
  sf::Text computerDifficulty;

};

#endif // GAMESCREEN_H
