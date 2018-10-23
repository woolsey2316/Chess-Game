#ifndef SQUARE_H
#define SQUARE_H

#include <vector>
#include <memory>
#include "ChessPiece.h"
typedef std::shared_ptr<ChessPiece> ChessPiecePtr;
/**
class Square represents a square on a chessboard that can only have 1 chess piece.
*/
class Square
{
  private:
  ChessPiecePtr chesspiece;
  bool vulnerableSquare;
  bool SQUARE_PROMOTING_PAWN;
  bool darkSquare;

  public:
  Square();
  Square(ChessPiecePtr piece);
  Square(ChessPiecePtr piece, bool topOrBottomSquare);
  bool isOccupied();
  ChessPiecePtr getChessPiece();
  bool canPromotePawn();
  void setChessPiece(Square& piece);
  void setChessPiece(ChessPiecePtr ptr);
  bool promotePawn(std::string ChessPieceType, std::string COLOUR);

};

#endif
