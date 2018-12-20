#include "Queen.h"
#include "ChessPiece.h"

Queen::Queen(std::string clr, std::string name)
  : ChessPiece(clr,name)
    {

    }

bool Queen::isValidMove(int x, int y, int dest_x, int dest_y)
{
  return
  //diagonal movement
  ((abs(dest_x - x) == abs(dest_y - y)) ||
    //vertical movement
    ((dest_y - y) == 0) & (abs(dest_x - x) > 0) ||
      //horizontal movement
      ((dest_x - x) == 0) & (abs(dest_y - y) > 0))
        &&!(dest_x == x && dest_y == y);
}

bool Queen::isValidMove(int x, int y, int dest_x,int dest_y,bool isCapturing)
{
  return
  //diagonal movement
  (abs(dest_x - x) == abs(dest_y - y)) ||
    //non-zero vertical movement
    ((dest_y - y) == 0 ) & (abs(dest_x - x) > 0) ||
      //non-zero horizontal movement
      ((dest_x - x) == 0) & (abs(dest_y - y) > 0);
}

bool Queen::isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove)
{
  return false;
}

bool Queen::isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  if (COLOUR != OTHER_COLOUR) {
    return isValidMove(x_pos, y_pos, x_dest, y_dest);
  }
  return false;
}

bool Queen::isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  return isValidCapture(x_pos, y_pos, x_dest, y_dest, OTHER_COLOUR);
}

std::vector<std::array<int, 2>> Queen::possibleMoves(int x_pos, int y_pos)
{
  for (int i = 1; i < 8; ++i) {
    if (validGridCoords(x_pos, y_pos, x_pos + i, y_pos)) {
      moveList.push_back(std::array<int, 2>{(x_pos + i + 8) % 8, y_pos});
    }
  }
  for (int i = 1; i < 8; ++i) {
    if (validGridCoords(x_pos, y_pos, (x_pos + i + 8) % 8, (y_pos - i + 8) % 8)) {
      moveList.push_back(std::array<int, 2>{(x_pos + i + 8) % 8, (y_pos - i + 8) % 8});
    }
  }
  for (int i = 1; i < 8; ++i) {
    if (validGridCoords(x_pos, y_pos, (x_pos + i + 8) % 8, (y_pos + i + 8) % 8)) {
      moveList.push_back(std::array<int, 2>{(x_pos + i + 8) % 8, (y_pos + i + 8) % 8});
    }
  }
  for (int i = 1; i < 8; ++i) {
    if (validGridCoords(x_pos, y_pos, x_pos, (y_pos + i) % 8)) {
      moveList.push_back(std::array<int, 2>{x_pos, (y_pos + i) % 8});
    }
  }
  return moveList;
}
