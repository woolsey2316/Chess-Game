#ifndef PAWN_H
#define PAWN_H

#include <array>
#include <string>
#include "ChessPiece.h"

class Pawn: public ChessPiece {
 public:
  Pawn(std::string clr, std::string name);
  bool isValidMove(int start_x, int start_y, int dest_x, int dest_y);
  bool isValidMove(int start_x, int start_y, int dest_x, int dest_y, bool isCapturing);
  bool isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove);
  bool isDefending(int x_pos, int y_pos, int x_dest, int y_dest, std::string COLOUR);
  bool isValidCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string COLOUR);
  std::vector<std::array<int, 2>> possibleMoves(int x_pos, int y_pos);
};

#endif // PAWN_H
