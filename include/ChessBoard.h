#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <string>
#include <vector>
#include <memory>
#include "ChessPiece.h"
#include "Square.h"
#include "Matrix.h"
#include "MoveType.h"
#include "CheckStatus.h"
#include <iostream>
#include "Coordinates.h"

class ChessBoard {
 private:
  int possibleMoves[8][8];
  Square board[8][8];
  int mostRecentMove[2];
  std::string promotingColour;
  std::string promotingPiece;
  int checkThreat_x, checkThreat_y;
  int kingIndex[2];
  CheckStatus checkStatus;
  MoveType moveType;

 public:
  ChessBoard();
  void reset();
  //determines the type of move (capture, move to empty square, checkmate, etc.) ,
  MoveType playerMove(std::string turn, int x, int y, int x_dest, int y_dest);
  MoveType captureChessPiece(int x_pos,int y_pos,int x_dest,int y_dest);
  //moves the king and rook at the same time acording to castling rules
  MoveType castle(int x_pos, int y_pos,int x_dest,int y_dest);
  MoveType moveToEmptySquare(int x_pos,int y_pos,int x_dest,int y_dest);
  // a special type of capture in chess where the pawn captures another pawn
  MoveType enPassant(int x_pos, int y_pos, int x_dest, int y_dest);
  bool validGridCoords(int x_pos, int y_pos, int x_dest, int y_dest);
  bool onStraightLine(int x_pos, int y_pos, int x_dest, int y_dest);
  bool legalCapture(int x_pos, int y_pos, int x_dest, int y_dest);
  bool doesMoveResolveCheck(int x_pos, int y_pos,int dest_x, int dest_y);
  bool isPinnedToKing(int x_pos, int y_pos);
  bool breaksThePin(int x_pos, int y_pos, int x_dest, int y_dest);
  bool isMoveLegal(int x_pos, int y_pos, int x_dest, int y_dest);
  //Determines if there is a chesspiece between source and destination of moving chesspiece
  bool isPathBlocked(int x_pos, int y_pos, int x_dest, int y_dest);
  // determines whether your move placed yourself into check and hence invalidate the move
  bool movedIntoCheck(int x_pos, int y_pos, int x_dest, int y_dest);
  // whether you checked, double checked, or checkmated the opponent
  CheckStatus determineCheckStatus(std::string COLOUR, bool opponentKing);
  //returns the index of the king
  void findKing(std::string COLOUR);
  // determines whether the king that is currently under check, can move to an adjacent square to avoid check.
  bool kingCanEscape(int* kingIndex, std::string COLOUR);
  // When in check can a piece move so that it stands between or captures the threat to the king
  bool canBlockorCaptureThreat(int x, int y, int threat_x, int threat_y);
  bool canBlockThreat(int x_pos, int y_pos);
  bool canCaptureThreat(int x_pos, int y_pos);
  // king and rook has never moved, no opponent is attacking a square between rook and king etc.
  bool isLegalCastle(int x_pos,int y_pos,int x_dest,int y_dest);
  // can a piece capture something on this position
  bool isSquareDefended(int x,int y);
  bool isDefended(int x, int y, std::string COLOUR);
  bool isSquareDefended(int x,int y,std::string COLOUR);
  bool isLegalCastle(int x_pos,int y_pos,int x_dest,int y_dest,std::string PLAYER_TURN);
  bool kingAttemptingCastle(int x_pos,int y_pos,int x_dest,int y_dest);
  MoveType promotePawn(int x_pos, int y_pos, int x_dest, int y_dest);
  void setPromotingColour(std::string PLAYER_TURN);
  void setPromotingPiece(std::string type);
  void findCheckStatus(std::string COLOUR);
  std::string getPromotingColour();
  std::string getPromotingPiece();
  ChessPiecePtr getChessPiece(int x, int y);
  std::string getType(int x, int y);
  std::string getColour(int x, int y);
  bool isOccupied(int x_pos, int y_pos);
  void testForStalemate(std::string PLAYER_TURN);
};

#endif