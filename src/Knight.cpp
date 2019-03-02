#include "Knight.h"
#include "ChessPiece.h"
#include <tuple>
#include <iostream>

Knight::Knight(std::string clr,std::string name)
  : ChessPiece(clr, name)
  {

  }
bool Knight::isValidMove(int x, int y, int dest_x, int dest_y)
{
  return ((dest_x-x)*(dest_x-x) + (dest_y-y)*(dest_y-y) == 5);
}

bool Knight::isValidMove(int x, int y, int dest_x, int dest_y,bool isCapturing)
{
  return ((dest_x-x)*(dest_x-x) + (dest_y-y)*(dest_y-y) == 5);
}

bool Knight::isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove)
{
  return false;
}

bool Knight::isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  if (COLOUR != OTHER_COLOUR) {
    return isValidMove(x_pos, y_pos, x_dest, y_dest);
  }
  return false;
}

bool Knight::isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  return isValidCapture(x_pos, y_pos, x_dest, y_dest, OTHER_COLOUR);
}

std::vector<std::array<int,2>> Knight::possibleMoves(int x_pos, int y_pos)
{
  moveList.clear();
  for (int xstep = 1; xstep <= 2; xstep++) {
    for (int ystep = 1; ystep <= 2; ystep++) {
      if (validGridCoords(x_pos, y_pos, x_pos+xstep, y_pos+ystep)) {
        moveList.push_back(std::array<int, 2>{x_pos+xstep, y_pos+ystep});
      }
      if (validGridCoords(x_pos, y_pos, x_pos-xstep, y_pos-ystep)) {
        moveList.push_back(std::array<int, 2>{x_pos-xstep, y_pos-ystep});
      }
    }
  }

  return moveList;
}


