#include "ChessPiece.h"
#include "Pawn.h"
#include <cmath>
#include <tuple>

using namespace std;

Pawn::Pawn(string clr, string name)
  :ChessPiece(clr, name){
}

bool Pawn::isValidMove(int x, int y, int dest_x, int dest_y){
  //move forward 1 space
  return ((dest_y-y == 0) &
    ((COLOUR == "WHITE") & (dest_x-x == -1)
      ||(COLOUR == "BLACK") & (dest_x-x == 1)))
  //pawn is allowed to move forward 2 spaces if it hasn't moved yet
  || (!this->moved() & (dest_y-y == 0) & ((COLOUR == "WHITE")
    & (dest_x-x == -2) ||(COLOUR == "BLACK") & (dest_x-x == 2)));
}

bool Pawn::isValidMove(int x, int y, int dest_x, int dest_y, bool isCapturing){
  //move forward 1 space
  bool regularMove = (dest_y-y == 0) &
    ((COLOUR == "WHITE") & (dest_x-x == -1)
      ||(COLOUR == "BLACK") & (dest_x-x == 1));
  //pawn is allowed to move forward 2 spaces if it hasn't moved yet
  bool twoStepMove = !this->moved() & (dest_y-y == 0) & ((COLOUR == "WHITE")
    & (dest_x-x == -2) ||(COLOUR == "BLACK") & (dest_x-x == 2));
  //can move diagonally 1 space if capturing
  bool captureMove = isCapturing &
    (((COLOUR == "WHITE") & (dest_x-x == -1) & (abs(dest_y-y) == 1))
      ||(COLOUR == "BLACK") & (dest_x-x == 1) & (abs(dest_y-y) == 1));
  return regularMove||twoStepMove||captureMove;
}

bool Pawn::isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  return (((COLOUR == "WHITE")
            &(abs(y_dest-y_pos) == 1) & (x_dest-x_pos == -1)
              ||(COLOUR == "BLACK")
                &(abs(y_dest-y_pos) == 1) & (x_dest-x_pos == 1)));
}

bool Pawn::isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove)
{
  return (recentMove[0] == start_x) & (recentMove[1] == dest_y)
      & (pieceType == "PAWN") &((start_x-dest_x)*(start_x-dest_x)
      + (dest_y-start_y)*(dest_y-start_y) == 2)
      //white enPassant
      & ((COLOUR == "WHITE") &((abs(dest_y-start_y) == 1)||(dest_y-start_y == 0))
      & (start_x == 3) & (dest_x-start_x == -1)
      //Black enPassant
      || (COLOUR == "BLACK") &((abs(dest_y-start_y) == 1)||(dest_y-start_y == 0))
      & (start_x == 4) & (dest_x-start_x == 1));

}

bool Pawn::isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string OTHER_COLOUR)
{
  return isValidCapture(x_pos, y_pos, x_dest, y_dest, OTHER_COLOUR);
}

std::vector<std::array<int, 2>> Pawn::possibleMoves(int x_pos, int y_pos)
{
  if (COLOUR == "WHITE") {
    //regular and two-step move
    for (int i = 1; i <= 2; ++i) {
      moveList.push_back(std::array<int, 2>{x_pos - i, y_pos});
    }
    //enPassant
    moveList.push_back(std::array<int, 2>{x_pos - 1, y_pos + 1});
    moveList.push_back(std::array<int, 2>{x_pos - 1, y_pos - 1});
  } else {
    for (int i = 1; i <= 2; ++i) {
      moveList.push_back(std::array<int, 2>{x_pos + i, y_pos});
    }
    //enPassant
    moveList.push_back(std::array<int, 2>{x_pos + 1, y_pos + 1});
    moveList.push_back(std::array<int, 2>{x_pos + 1, y_pos - 1});
  }
  return moveList;
}
