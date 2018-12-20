#ifndef CHESSBOARDSCREEN_H
#define CHESSBOARDSCREEN_H

#include "MoveType.h"
#include "ChessBoard.h"
#include "Connector.h"
#include "Coordinates.h"
#include "ChessGame.h"
#include "main.h"
#include "cScreen.h"
#include <map>

class ChessBoardScreen : public cScreen
{
 public:
  void loadScreen();
  virtual void refreshDisplay(sf::RenderWindow &App) = 0;
  void reverseMove(sf::RenderWindow &App);
  void loadPosition(sf::RenderWindow &App);
  int Run(sf::RenderWindow &App);
  size_t getChessBoardSquareSize();
  void move(Coordinates oldCoords, Coordinates newCoords, MoveType moveType, int touchedByPlayer);
  void playerMove(sf::Vector2f start, sf::Vector2f dest, MoveType moveType);
  void computerMove();
  void promotePawn(int index, sf::Vector2f dest);
  MoveType letGoOfPiece();
  void dragChessPiece();
  void moveSprite(MoveType moveType);
  ChessBoard getChessBoard();
  Computer comp;
  void setChessBoardFile(std::string file);
  void loadChessSprites();
  void getPositionScores();
  void resetGame();

 protected:
  std::string chessBoardFile = "images/chessboard2.png";
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
  sf::Sprite settings;
  sf::Texture settings_t;
  sf::Texture mainMenu_t;
  sf::Sprite mainMenu;
  sf::Texture retry_t;
  sf::Sprite retry;
  std::array<sf::Texture, 32> texture;
  std::array<sf::Sprite, 32> chesspieceSprite;
  std::vector<sf::Text> variation;
  std::array<sf::RectangleShape, 2> rectangle;

  sf::Font Font;
};

#endif // CHESSBOARDSCREEN_H
