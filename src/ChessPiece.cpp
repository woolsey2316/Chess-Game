#include <string>
#include "ChessPiece.h"

ChessPiece::ChessPiece(std::string clr, std::string type_)
{
  COLOUR = clr;
  type = type_;
  hasMoved = false;
  totalMoves = 0;
}

std::string ChessPiece::getColour()
{
  return COLOUR;
}

std::string ChessPiece::getType()
{
  return type;
}

bool ChessPiece::moved()
{
  return hasMoved;
}

void ChessPiece::setHasMoved(bool moved)
{
  hasMoved = moved;
}

void ChessPiece::incrementTotalMoves()
{
  totalMoves++;
}

int ChessPiece::getTotalMoves()
{
  return totalMoves;
}

bool ChessPiece::matingPiece()
{
  return (type == "ROOK") || (type == "QUEEN") || (type == "PAWN");
}

bool ChessPiece::knightOrBishop()
{
  return (type == "KNIGHT") || (type == "BISHOP");
}

bool ChessPiece::validGridCoords(int x_pos, int y_pos, int x_dest, int y_dest)
{
  return (x_pos >= 0 && x_pos < 8 && y_pos >= 0 && y_pos < 8) &&
      (x_dest >= 0 && x_dest < 8 && y_dest >= 0 && y_dest < 8);
}
