#ifndef ROOK_H
#define ROOK_H

#include <array>
#include <string>
#include "ChessPiece.h"

class Rook: public ChessPiece {
 public:
  Rook(std::string clr, std::string name);
  bool isValidMove(int x, int y, int dest_x,int dest_y);
  bool isValidMove(int x, int y, int dest_x, int dest_y, bool iscapturing);
  bool isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove);
  bool isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string COLOUR);
  bool isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string COLOUR);
  std::vector<std::array<int, 2>> possibleMoves(int x_pos, int y_pos);
};

#endif
