#include <string>
#include "King.h"
#include "ChessPiece.h"
#include <vector>
#include <tuple>
//constructor
King::King(std::string clr,std::string name)
  : ChessPiece(clr,name)
  {
    hasMoved = false;
    inCheck = false;
  }

bool King::isValidMove(int x, int y, int dest_x, int dest_y)
{
  return ((dest_x-x)*(dest_x-x) + (dest_y-y)*(dest_y-y)==1 ||
          (dest_x-x)*(dest_x-x) + (dest_y-y)*(dest_y-y)==2);
}

bool King::isValidMove(int x, int y, int dest_x, int dest_y, bool iscapturing)
{
  return false;
}

bool King::isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove)
{
  return false;
}

bool King::isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  if (COLOUR != OTHER_COLOUR) {
    return isValidMove(x_pos, y_pos, x_dest, y_dest);
  }
  return false;
}

bool King::isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  return isValidCapture(x_pos, y_pos, x_dest, y_dest, OTHER_COLOUR);
}

std::vector<std::array<int, 2>> King::possibleMoves(int x_pos, int y_pos)
{
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      if (i == 0 && j == 0) {
        continue;
      }
    if (validGridCoords(x_pos, y_pos, x_pos + i, y_pos + j))
      moveList.push_back(std::array<int, 2>{x_pos + i, y_pos + j});
    }
  }

  return moveList;
}

