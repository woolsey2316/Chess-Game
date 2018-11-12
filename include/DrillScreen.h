#ifndef DRILLSCREEN_H
#define DRILLSCREEN_H

#include "MoveType.h"
#include "ChessBoard.h"
#include "Connector.h"
#include "Coordinates.h"
#include "ChessGame.h"
#include "main.h"
#include "cScreen.h"

extern const std::string board_[8][8];

class DrillScreen : public cScreen {
 public:
  DrillScreen(void);
	virtual int Run(sf::RenderWindow &App);
  size_t getChessBoardSquareSize();
  void move(Coordinates oldCoords, Coordinates newCoords, MoveType moveType, int touchedByPlayer);
  void playerMove(sf::Vector2f start, sf::Vector2f dest, MoveType moveType);
  void loadScreen();
  void loadChessSprites();
  void computerMove();
  void promotePawn(int index, sf::Vector2f dest);
  MoveType letGoOfPiece();
  void dragChessPiece();
  void moveSprite(MoveType moveType);
  void loadPosition(sf::RenderWindow &App);
  void refreshDisplay(sf::RenderWindow &App);
  ChessBoard getChessBoard();
  Computer comp;

 private:
  std::string filename;
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
  std::string drill[8] = {"Queen And King",
      "Two Rooks", "Two Bishops","Rook Mate",
      "Bishop And Knight", "Queen Vs Bishop",
      "Queen Vs Knight", "Queen Vs Rook"};

  sf::Texture background_t;
  sf::Sprite background;
  sf::Texture sidePanel_t;
  sf::Sprite sidePanel;
  sf::Texture bottomPanel_t;
  sf::Sprite bottomPanel;
  sf::Texture gameBoard_t;
  sf::Sprite boardSprite;
  sf::Sprite backButton;
  sf::Texture backButton_t;
  sf::Sprite forwardButton;
  sf::Texture forwardButton_t;
  std::array<sf::Texture, 32> texture;
  std::array<sf::Sprite, 32> chesspieceSprite;

  std::array<sf::Text, 8> drillType;
  sf::Font Font;
};

#endif // DRILLSCREEN_H
