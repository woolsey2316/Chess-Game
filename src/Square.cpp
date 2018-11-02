#include <vector>
#include "Square.h"
#include "ChessPiece.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"

Square::Square()
{
  vulnerableSquare = false;
  chesspiece = NULL;
  SQUARE_PROMOTING_PAWN = false;
}

Square::Square(ChessPiecePtr piece)
{
  vulnerableSquare = false;
  chesspiece = piece;
  SQUARE_PROMOTING_PAWN = false;
}

Square::Square(ChessPiecePtr piece, bool topOrBottomSquare)
{
  vulnerableSquare = false;
  chesspiece = piece;
  SQUARE_PROMOTING_PAWN = topOrBottomSquare;
}

bool Square::isOccupied()
{
  return (chesspiece != NULL);
}

ChessPiecePtr Square::getChessPiece()
{
  return chesspiece;
}

void Square::setChessPiece(Square& sq)
{
  chesspiece = std::move(sq.chesspiece);
}

bool Square::promotePawn(std::string type, std::string COLOUR)
{
  if (type=="queen") {
    ChessPiecePtr ptr(new Queen(COLOUR,type));
    chesspiece = std::move(ptr);
    return true;
  } else if (type=="knight") {
    ChessPiecePtr ptr(new Knight(COLOUR,type));
    chesspiece = std::move(ptr);
    return true;
  } else if (type=="bishop") {
    ChessPiecePtr ptr(new Bishop(COLOUR,type));
    chesspiece = std::move(ptr);
    return true;
  } else if (type=="rook") {
    ChessPiecePtr ptr(new Rook(COLOUR,type));
    chesspiece = std::move(ptr);
    return true;
  }
  return false;

}

void Square::setChessPiece(ChessPiecePtr ptr)
{
  chesspiece = std::move(ptr);
}

bool Square::canPromotePawn()
{
  return SQUARE_PROMOTING_PAWN;
}

String getColour()
{
  return COLOUR;
}
