#ifndef ANALYSISSCREEN_H
#define ANALYSISSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "MoveType.h"
#include "ChessBoard.h"
#include "Connector.h"
#include "Coordinates.h"
#include "ChessGame.h"
#include "main.h"
#include "cScreen.h"

extern const std::string board_[8][8];

class AnalysisScreen : public cScreen {
 public:
  AnalysisScreen(void);
	virtual int Run(sf::RenderWindow &App);
  size_t getChessBoardSquareSize();
  void move(Coordinates oldCoords, Coordinates newCoords, MoveType moveType, int touchedByPlayer);
  void playerMove(sf::Vector2f start, sf::Vector2f dest, MoveType moveType);
  void loadChessSprites();
  void computerMove();
  void promotePawn(int index, sf::Vector2f dest);
  MoveType letGoOfPiece();
  void dragChessPiece();
  void moveSprite(MoveType moveType);
  void reverseMove(sf::RenderWindow &App);
  void refreshDisplay(sf::RenderWindow &App);
  ChessBoard getChessBoard();
  Computer comp;

 private:
  ChessGame chessgame;
  bool canReverseMoves;
  Connector connector;
  ChessBoard chessboard;
  sf::Vector2i mousePosition;
  bool isPressed = false;
  bool buttonPressed = false;
  Coordinates oldCoordinates;
  Coordinates newCoordinates;
  MoveType moveType;
  sf::Vector2f oldPos;
  sf::Vector2f newPos;
  int touchedByPlayer = -1;
  int touchedByComputer = -1;
  float dx = 0, dy = 0;

  sf::Texture sidePanel_t;
  sf::Sprite sidePanel;
  sf::Texture gameBoard_t;
  sf::Sprite boardSprite;
  sf::Sprite backButton;
  sf::Texture backButton_t;
  sf::Sprite forwardButton;
  sf::Texture forwardButton_t;
  sf::Texture texture[32];
  sf::Sprite chesspieceSprite[32];

};

#endif // ANALYSISSCREEN_H
