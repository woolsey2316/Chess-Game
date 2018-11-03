#include "Bishop.h"
#include <cmath>
#include <tuple>
using namespace std;

Bishop::Bishop(std::string clr, string name)
  : ChessPiece(clr,name)
{

}

Bishop::~Bishop()
{
  //dtor
}

bool Bishop::isValidMove(int x, int y, int dest_x,int dest_y)
{
  return (abs(dest_x - x) == abs(dest_y - y))&&!(dest_x == x && dest_y == y);
}

bool Bishop::isValidMove(int x, int y, int dest_x,int dest_y,bool isCapturing)
{
  return (abs(dest_x - x) == abs(dest_y - y));
}

bool Bishop::isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove)
{
  return false;
}

bool Bishop::isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  if (COLOUR != OTHER_COLOUR) {
    return isValidMove(x_pos, y_pos, x_dest, y_dest);
  }
  return false;
}

bool Bishop::isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  return isValidCapture(x_pos, y_pos, x_dest, y_dest, OTHER_COLOUR);
}

std::vector<std::array<int,2>> Bishop::possibleMoves(int x_pos, int y_pos)
{
  for (int i = 1; i < 8; ++i) {
    if (validGridCoords(x_pos, y_pos, x_pos + i + 8 % 8, y_pos - i + 8 % 8)) {
      moveList.push_back(std::array<int, 2>{x_pos + i + 8 % 8, y_pos - i + 8 % 8});
    }
  }
  for (int i = 1; i < 8; ++i) {
    if (validGridCoords(x_pos, y_pos, x_pos + i + 8 % 8, y_pos + i + 8 % 8)) {
      moveList.push_back(std::array<int, 2>{x_pos + i + 8 % 8, y_pos + i + 8 % 8});
    }
  }
  return moveList;
}
