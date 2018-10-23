#include "Rook.h"
#include "ChessPiece.h"

Rook::Rook(std::string clr,std::string name)
  : ChessPiece(clr, name)
  {
    hasMoved = false;
  }
bool Rook::isValidMove(int x, int y, int dest_x,int dest_y)
{
  return !((dest_x - x == 0) & (dest_y - y == 0))&
          (dest_x - x == 0 || dest_y - y == 0);
}

bool Rook::isValidMove(int x, int y, int dest_x,int dest_y,bool isCapturing)
{
  return !((dest_x - x == 0) & (dest_y - y == 0))&
          (dest_x - x == 0 || dest_y - y == 0);
}

bool Rook::isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove)
{
  return false;
}

bool Rook::isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  if (COLOUR != OTHER_COLOUR) {
    return isValidMove(x_pos, y_pos, x_dest, y_dest);
  }
  return false;
}

bool Rook::isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  return isValidCapture(x_pos, y_pos, x_dest, y_dest, OTHER_COLOUR);
}

std::vector<std::array<int, 2>> Rook::possibleMoves(int x_pos, int y_pos)
{
  for (int i = 1; i < 8; ++i) {
    moveList.push_back(std::array<int, 2>{x_pos + i % 8, y_pos});
  }
  for (int i = 1; i < 8; ++i) {
    moveList.push_back(std::array<int, 2>{x_pos, y_pos + i % 8});
  }
  return moveList;
}

