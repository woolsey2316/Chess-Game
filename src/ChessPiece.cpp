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
