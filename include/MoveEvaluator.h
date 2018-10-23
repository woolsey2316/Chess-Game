#ifndef MOVEEVALUATOR_H
#define MOVEEVALUATOR_H

#include "MoveType.h"
#include "ChessBoard.h"
class MoveEvaluator {
 public:
  MoveEvaluator();
  virtual ~MoveEvaluator();
 private:
  bool legalCapture(int x_pos, int y_pos, int x_dest, int y_dest);
  bool doesMoveResolveCheck(int x_pos, int y_pos,int dest_x, int dest_y, std::string COLOUR);
  bool isThreatRemoverPinnedToKing(int x_pos, int y_pos);
  MoveType captureChessPiece(int x_pos, int y_pos, int x_dest, int y_dest);
  bool isMoveLegal(int x_pos, int y_pos, int x_dest, int y_dest);
  //Determines if there is a chesspiece between source and destination of moving chesspiece
  bool isPathBlocked(int x_pos, int y_pos, int x_dest, int y_dest);
  // determines whether your move placed yourself into check and hence invalidate the move
  bool movedIntoCheck(std::string COLOUR);
  // whether you checked, double checked, or checkmated the opponent
  CheckStatus determineCheckStatus(std::string COLOUR, bool opponentKing);
  //returns the index of the king
  void findKing(std::string COLOUR);
  // determines whether the king that is currently under check, can move to an adjacent square to avoid check.
  bool kingCanEscape(int* kingIndex, std::string COLOUR);
  // When in check can a piece move so that it stands between or captures the threat to the king
  bool canBlockorCaptureThreat(int x, int y, std::string COLOUR);
  // king and rook has never moved, no opponent is attacking a square between rook and king etc.
  bool isLegalCastle(int x_pos,int y_pos,int x_dest,int y_dest);
  // can a piece capture something on this position
  bool isVulnerableSquare(int x,int y);
  bool isVulnerableSquare(int x,int y,std::string COLOUR);
  bool isLegalCastle(int x_pos,int y_pos,int x_dest,int y_dest,std::string COLOUR);
  bool kingAttemptingCastle(int x_pos,int y_pos,int x_dest,int y_dest,bool notMoved);

};

#endif // MOVEEVALUATOR_H
