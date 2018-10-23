#ifndef KING_H
#define KING_H

#include <array>
#include <string>
#include "ChessPiece.h"

class King: public ChessPiece {
 public:
  King(std::string clr, std::string name);
  bool inCheck;
  bool hasMoved;
  bool isValidMove(int x, int y, int dest_x,int dest_y);
  bool isValidMove(int x, int y, int dest_x, int dest_y, bool iscapturing);
  bool isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove);
  bool isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string COLOUR);
  bool isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string COLOUR);
  std::vector<std::array<int, 2>> possibleMoves(int x_pos, int y_pos);
};

#endif // KING_H
