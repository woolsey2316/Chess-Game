#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "MoveType.h"
#include "ChessBoard.h"
#include "Connector.h"
#include "Coordinates.h"
#include "ChessGame.h"
#include "main.h"

class Display {
 public:
  Display();
  size_t getChessBoardSquareSize();
  void move(Coordinates oldCoords, Coordinates newCoords, MoveType moveType, int touchedByPlayer);
  void playerMove(sf::Vector2f start, sf::Vector2f dest, MoveType moveType);
  void loadChessSprites();
  void computerMove();
  void promotePawn(int index, sf::Vector2f dest);
  MoveType letGoOfPiece();
  void dragChessPiece();
  void updateDisplay(MoveType moveType);
  void reverseMove();
  ChessBoard getChessBoard();

 private:
  ChessGame chessgame;
  Connector connector;
  ChessBoard chessboard;
  sf::Vector2i mousePosition;
  bool isStillMoving = false;
  Coordinates oldCoordinates;
  Coordinates newCoordinates;
  MoveType moveType;
  sf::Vector2f oldPos;
  sf::Vector2f newPos;
  int touchedByPlayer = -1;
  int touchedByComputer = -1;
  float dx = 0, dy = 0;
  sf::Texture gameBoard;
  sf::Sprite boardSprite;
  sf::Texture texture[32];
  sf::Sprite chesspieceSprite[32];
  std::string board_[8][8] = {
      {"black_rook","black_knight", "black_bishop", "black_queen", "black_king", "black_bishop","black_knight","black_rook"},
      {"black_pawn", "black_pawn" , "black_pawn"  , "black_pawn" , "black_pawn" ,  "black_pawn" , "black_pawn" ,"black_pawn"},
      {  "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,     "empty"},
      {  "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,     "empty"},
      {  "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,     "empty"},
      {  "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,     "empty"},
      {"white_pawn", "white_pawn" , "white_pawn"  , "white_pawn" , "white_pawn" ,  "white_pawn" , "white_pawn" ,"white_pawn"},
      {"white_rook","white_knight", "white_bishop", "white_queen", "white_king", "white_bishop","white_knight","white_rook"}
      };



};

#endif // DISPLAY_H
