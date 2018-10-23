#include "Pawn.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "King.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Square.h"
#include "Matrix.h"
#include "CheckMateScreen.h"
#include "PromotionScreen.h"


#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <memory>
#include <iostream>

typedef std::shared_ptr<ChessPiece> ChessPiecePtr;

bool validPawnCapture(int x_pos, int y_pos, int x_dest, int y_dest, std::string COLOUR);
//default constructor initialises pieces in starting position
ChessBoard::ChessBoard()
{
  reset();
}

MoveType ChessBoard::playerMove(std::string playerTurn, int x_pos, int y_pos, int x_dest, int y_dest)
{
  moveType = MoveType::Invalid;
  if (!validGridCoords(x_pos, y_pos, x_dest, y_dest)) {
    return moveType;
  }
  //user didn't select a piece to move was an empty square
  if (!board[x_pos][y_pos].isOccupied()){
    return moveType;
  }
  //didn't even move
  if (x_pos == x_dest && y_pos == y_dest) {
    return moveType;
  }
  //checks if it is the player's turn
  if (board[x_pos][y_pos].getChessPiece()->getColour() != playerTurn){
    return moveType;
  }
  std::string chessPieceType = board[x_pos][y_pos].getChessPiece()->getType();
  if (chessPieceType == "KING") {
    kingIndex[0] = x_dest; kingIndex[1] = y_dest;
  }
  std::string COLOUR = board[x_pos][y_pos].getChessPiece()->getColour();
  determineCheckStatus(playerTurn, false);

  if (checkStatus == CheckStatus::Check) {
    if (!doesMoveResolveCheck(x_pos, y_pos, x_dest, y_dest)) {
      return moveType;
    }
  }
    //A move that involves a capture or castling
    if (board[x_dest][y_dest].isOccupied()) {
      //check if piece captures the other colour
      moveType = captureChessPiece(x_pos, y_pos, x_dest, y_dest);
      if (moveType == MoveType::Invalid) {
        //Pawn has its own capture method
        if (board[x_pos][y_pos].getChessPiece()->isValidCapture(
              x_pos, y_pos, x_dest, y_dest, COLOUR)) {
            moveType = captureChessPiece(x_pos, y_pos, x_dest, y_dest);
        }
      }
    } else if (kingAttemptingCastle(x_pos, y_pos, x_dest, y_dest)) {
        moveType = (castle(x_pos, y_pos, x_dest, y_dest));
    } else if (chessPieceType == "PAWN") {
        moveType = enPassant(x_pos, y_pos, x_dest, y_dest);
    }
    if (moveType == MoveType::Invalid) {
      moveType = moveToEmptySquare(x_pos, y_pos, x_dest, y_dest);
    }
    if (moveType != MoveType::Invalid) {
      board[x_dest][y_dest].getChessPiece()->setHasMoved(true);
      board[x_dest][y_dest].getChessPiece()->incrementTotalMoves();
      mostRecentMove[0] = x_dest;
      mostRecentMove[1] = y_dest;

      if (chessPieceType == "PAWN" && board[x_dest][y_dest].canPromotePawn()) {
        moveType = promotePawn(x_pos, y_pos, x_dest, y_dest);
      }
      bool opponentKing = true;
      if (determineCheckStatus(playerTurn, opponentKing) == CheckStatus::CheckMate) {
        return MoveType::CheckMate;
        std::cout << "Check Mate" << playerTurn << "Wins!" << std::endl;
      }
      if (checkStatus == CheckStatus::NotInCheck) {
        testForStalemate(playerTurn);
      }
    }
    return moveType;
}

bool ChessBoard::doesMoveResolveCheck(int x_pos, int y_pos, int x_dest, int y_dest)
{
  std::string PLAYER_TURN = board[x_pos][y_pos].getChessPiece()->getColour();
  if (checkStatus == CheckStatus::NotInCheck) {
    return true;
  } else if (checkStatus == CheckStatus::Check) {
    if (kingIndex[0] == x_pos && kingIndex[1] == y_pos) {
      return !isSquareDefended(x_dest, y_dest, PLAYER_TURN);
    }
    if (isPinnedToKing(x_pos, y_pos)) {
      if (!onStraightLine(x_pos, y_pos, x_dest, y_dest)) {
        return false;
      }
    }
    return (canBlockorCaptureThreat(x_pos, y_pos, x_dest, y_dest));
  } else if (checkStatus == CheckStatus::DoubleCheck) {
    //must move king when in double check
    if (board[x_pos][y_pos].getChessPiece()->getType() == "KING") {
      if (isSquareDefended(x_dest, y_dest, PLAYER_TURN)) {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

bool ChessBoard::isPinnedToKing(int x_pos, int y_pos)
{
  int x = x_pos; int y = y_pos;
  if (!onStraightLine(x_pos, y_pos, kingIndex[0], kingIndex[1])) {
    return false;
  }
  if (isPathBlocked(x_pos, y_pos, kingIndex[0], kingIndex[1])) {
    return false;
  }
  int x_step = (x_pos - kingIndex[0]) == 0 ? 0 : (x_pos - kingIndex[0])/abs(x_pos - kingIndex[0]);
  int y_step = (y_pos - kingIndex[1]) == 0 ? 0 : (y_pos - kingIndex[1])/abs(y_pos - kingIndex[1]);

  //searching for a threat to king on the same line as piece and king
  while (x_pos < 7 && x_pos > 0 && y_pos < 7 && y_pos > 0) {
    x_pos -= x_step;
    y_pos -= y_step;
    if (board[x_pos][y_pos].isOccupied()) {
      //check if piece exists between it and king
      if (!isPathBlocked(x_pos, y_pos, x, y)) {
        if (board[x_pos][y_pos].getChessPiece()->getType() != "PAWN" &&
            board[x_pos][y_pos].getChessPiece()->getType() != "KNIGHT") {
          if (board[x_pos][y_pos].getChessPiece()->isDefending(x_pos, y_pos, x, y,
                                                board[x_pos][y_pos].getChessPiece()->getColour())) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool ChessBoard::breaksThePin(int x_pos, int y_pos, int x_dest, int y_dest)
{
  int x_step = (x_pos - kingIndex[0]) == 0 ? 0 : (x_pos - kingIndex[0])/abs(x_pos - kingIndex[0]);
  int y_step = (y_pos - kingIndex[1]) == 0 ? 0 : (y_pos - kingIndex[1])/abs(y_pos - kingIndex[1]);

  while (x_pos < 7 && x_pos > 0 && y_pos < 7 && y_pos > 0) {
    x_pos -= x_step;
    y_pos -= y_step;
    if (x_pos == x_dest && y_pos == y_dest) {
      return false;
    }
  }
  return true;
}

MoveType ChessBoard::promotePawn(int x_pos, int y_pos, int x_dest, int y_dest)
{
  std::string COLOUR = board[x_dest][y_dest].getChessPiece()->getColour();
  PromotionScreen promotionScreen(COLOUR);
  std::string s = promotionScreen.getPromotionOption();
  std::string delimiter = "_";

  size_t pos = 0;
  std::string pieceType;
  while ((pos = s.find(delimiter)) != std::string::npos) {
      pieceType = s.substr(pos+1, s.length());
      s.erase(0, pos + delimiter.length());
  }
    board[x_dest][y_dest].promotePawn(pieceType, COLOUR);
  setPromotingColour(board[x_dest][y_dest].getChessPiece()->getColour());
  setPromotingPiece(pieceType);
  return MoveType::Promotion;
}

MoveType ChessBoard::moveToEmptySquare(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way and whether
  //the piece moved validly according its own move rules
  if (isMoveLegal(x_pos, y_pos, x_dest, y_dest)) {
    //move where no capturing of chesspiece
    board[x_dest][y_dest].setChessPiece(board[x_pos][y_pos]);
    return MoveType::EmptySquare;
  }

  return MoveType::Invalid;
}

MoveType ChessBoard::enPassant(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way and whether
  //the piece moved validly according its own move rules
  if (board[x_pos][y_dest].isOccupied()) {
    std::string pieceType = board[x_pos][y_dest].getChessPiece()->getType();
    ChessPiecePtr debug = board[x_pos][y_pos].getChessPiece();
    if (board[x_pos][y_pos].getChessPiece()->isValidEnPassant(
            x_pos, y_pos, x_dest, y_dest, pieceType, mostRecentMove)) {
      board[x_dest][y_dest].setChessPiece(board[x_pos][y_pos]);
      board[x_pos][y_dest].setChessPiece(NULL);
      return MoveType::enPassant;
    }
  }
  return MoveType::Invalid;
}

bool ChessBoard::kingAttemptingCastle(int x_pos, int y_pos, int x_dest, int y_dest)
{
  return (board[x_pos][y_pos].getChessPiece()->getType() == "KING"
          && x_pos-x_dest==0
          && abs(y_dest - y_pos)==2)
          && (!board[x_pos][y_pos].getChessPiece()->moved());
}

MoveType ChessBoard::castle(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way and whether
  //the piece moved validly according its own move rules
  std::string COLOUR = board[x_pos][y_pos].getChessPiece()->getColour();
  if (isLegalCastle(x_pos, y_pos, x_dest, y_dest, COLOUR)) {
      //update king location here
      board[x_dest][y_dest].setChessPiece(board[x_pos][y_pos]);
      //kingside castle update rook position
      if (y_dest==6){
        board[x_pos][5].setChessPiece(board[x_pos][7]);
        board[x_pos][5].getChessPiece()->setHasMoved(true);
      //queenside castle update rook position
      } else if (y_dest==2){
        board[x_pos][3].setChessPiece(board[x_pos][0]);
        board[x_pos][3].getChessPiece()->setHasMoved(true);
      }
        return MoveType::Castle;
  }
  return MoveType::Invalid;
}

bool ChessBoard::isLegalCastle(int x_pos,int y_pos,int x_dest,int y_dest,std::string COLOUR)
{
  //finds whether kingside or queenside castling is attempted
  int moveRightorLeft = (y_dest == 6 ? 1 : -1);
  bool queenSide = (moveRightorLeft == 1 ? 0 : 1);
  //check if both king and rook have not moved,
  if (queenSide){
    if (board[x_pos][0].getChessPiece()->moved())
        return false;
    } else if (board[x_pos][7].getChessPiece()->moved()){
      return false;
    }
  //check to see if King is in check
  if (isDefended(x_pos, y_pos, COLOUR))
    return false;

  //checks if any square the king passes through is vulnerable to attack
  for (int i=0;i<2;++i) {
    y_pos+=moveRightorLeft;
    if (isDefended(x_pos, y_pos, COLOUR))
      return false;
  }
  //castling by queenside requires checking 1 more square
  if (queenSide){
    if (board[x_pos][y_pos-1].isOccupied())
      return false;
  }
  return true;
}

MoveType ChessBoard::captureChessPiece(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way and whether
  //the piece moved validly according its own move rules
  if (isMoveLegal(x_pos, y_pos, x_dest, y_dest)) {
      board[x_dest][y_dest].setChessPiece(board[x_pos][y_pos]);
      return MoveType::Capture;
  }
  return MoveType::Invalid;
}
/**
tests to see whether the game is in check
*/
CheckStatus ChessBoard::determineCheckStatus(std::string COLOUR, bool opponentKing)
{
  std::string OPPONENT_COLOUR;
  if (opponentKing) {
    COLOUR = (COLOUR == "WHITE" ? "BLACK" : "WHITE");
  }
  OPPONENT_COLOUR = (COLOUR == "WHITE" ? "BLACK" : "WHITE");

  //iterate through whole chessboard to find the King
  findKing(COLOUR);
  //find if the chessboard is in check and the threats
  checkStatus = CheckStatus::NotInCheck;
  //iterate through whole chessboard to see if any piece can capture the king
  for (int x=0;x<8;++x) {
    for (int y=0;y<8;++y) {
      if (board[x][y].isOccupied() && board[x][y].getChessPiece()->getColour() != COLOUR) {
        if (!isPathBlocked(x, y, kingIndex[0], kingIndex[1]))
          if (board[x][y].getChessPiece()->isValidCapture(x, y, kingIndex[0], kingIndex[1], COLOUR)) {
            if (checkStatus == CheckStatus::NotInCheck){
              checkStatus = CheckStatus::Check;
            }
            else {
              checkStatus = CheckStatus::DoubleCheck;
            }
            checkThreat_x = x;
            checkThreat_y = y;
          }
      }
    }
  }

  if (checkStatus == CheckStatus::Check) {
    if(!kingCanEscape(kingIndex, COLOUR)) {
      for (int x_pos=0;x_pos<8;++x_pos) {
        for (int y_pos=0;y_pos<8;++y_pos) {
          if (board[x_pos][y_pos].isOccupied()) {
            if (canBlockThreat(x_pos, y_pos) || canCaptureThreat(x_pos, y_pos)) {
              return CheckStatus::Check;
            }
          }
        }
      }
      std::cout << "Check Mate" << std::endl;
      CheckMateScreen CheckMateScreen;
      return CheckStatus::CheckMate;
    }
  } else if (checkStatus == CheckStatus::DoubleCheck) {
    if (!kingCanEscape(kingIndex, OPPONENT_COLOUR)) {
      CheckMateScreen CheckMateScreen;
      return CheckStatus::CheckMate;
    }
  }
  return checkStatus;

}

void ChessBoard::findKing(std::string COLOUR)
{
  for (int i=0;i<8;++i) {
    for (int j=0;j<8;++j) {
      if (board[i][j].isOccupied()) {
        if (board[i][j].getChessPiece()->getType() == "KING"
            && (board[i][j].getChessPiece()->getColour()==COLOUR)){
              kingIndex[0] = i;
              kingIndex[1] = j;
        }
      }
    }
  }
}

bool ChessBoard::movedIntoCheck(int x_pos, int y_pos, int x_dest, int y_dest)
{
  std::string COLOUR = board[x_pos][y_pos].getChessPiece()->getColour();
  findKing(COLOUR);
  if (x_pos == kingIndex[0] && y_pos == kingIndex[1]) {
    return isSquareDefended(x_dest, y_dest, COLOUR);
  }
  for (int x=0;x<8;++x) {
    for (int y=0;y<8;++y) {
      if (board[x][y].isOccupied() && board[x][y].getChessPiece()->getColour() != COLOUR) {
        if (!isPathBlocked(x, y, kingIndex[0], kingIndex[1]))
          if (board[x][y].getChessPiece()->isValidCapture(x, y, kingIndex[0], kingIndex[1], COLOUR)) {
            checkThreat_x = x;
            checkThreat_y = y;
          }
      }
    }
  }

  if (isPinnedToKing(x_pos, y_pos)) {
    if (breaksThePin(x_pos, y_pos, x_dest, y_dest)) {
      return true;
    }
  }
  return false;
}

bool ChessBoard::canBlockorCaptureThreat(int x_pos, int y_pos, int x_dest, int y_dest)
{
  std::string COLOUR = board[x_pos][y_pos].getChessPiece()->getColour();
  std::string OPPONENT_COLOUR = (COLOUR == "WHITE" ? "BLACK" : "WHITE");
  //3 cases diagonal threat, horizontal threat, vertical threat
  if (!isPathBlocked(x_pos, y_pos, x_dest, y_dest)) {
    if (x_dest == checkThreat_x && y_dest == checkThreat_y) {
      if (!board[x_pos][y_pos].getChessPiece()->isValidCapture(
              x_pos, y_pos, x_dest, y_dest, OPPONENT_COLOUR )) {
        return false;
      }
    } else if (!board[x_pos][y_pos].getChessPiece()->isValidMove(
              x_pos, y_pos, x_dest, y_dest)) {
        return false;
      }
      if (kingIndex[0] - checkThreat_x == 0) {
        return (x_dest == kingIndex[0])
            && (((kingIndex[1] - y_dest < 0)
            && (checkThreat_y >= y_dest))
            || ((kingIndex[1] - y_dest > 0)
            && (checkThreat_y <= y_dest)));
      } else if (kingIndex[1] - checkThreat_y == 0) {
        return (y_dest == kingIndex[1])
           && (((kingIndex[0] - x_dest < 0)
           && (checkThreat_x >= x_dest))
           || ((kingIndex[0] - x_dest > 0)
           && (checkThreat_x <= x_dest)));
      } else if (abs(kingIndex[1] - checkThreat_y) == abs(kingIndex[0] - checkThreat_x)){
        return !(((x_dest > kingIndex[0]) && (x_dest > checkThreat_x))
            || ((x_dest < kingIndex[0]) && (x_dest < checkThreat_x)))
            && !(((y_dest > kingIndex[1]) && (y_dest > checkThreat_y))
            || ((y_dest < kingIndex[1]) && (y_dest < checkThreat_y)))
            && abs(x_dest - kingIndex[0]) == abs(y_dest - kingIndex[1]);
      } else {
        //knight check can't be blocked
        return (board[x_pos][y_pos].getChessPiece()->isValidCapture(
            x_pos, y_pos, checkThreat_x, checkThreat_y, OPPONENT_COLOUR));
      }
    }
  return false;
}

bool ChessBoard::canBlockThreat(int x_pos, int y_pos)
{
  if (kingIndex[0] == x_pos && kingIndex[1] == y_pos) {
    return false;
  }
  int y_dest, x_dest;
  std::vector<std::array<int, 2>> possibleMoves = board[x_pos][y_pos].getChessPiece()->possibleMoves(x_pos, y_pos);
  for (const auto & dest : possibleMoves) {
      x_dest = dest[0]; y_dest = dest[1];
    //3 cases diagonal threat, horizontal threat, vertical threat
    if (!isPathBlocked(x_pos, y_pos, x_dest, y_dest)) {
      if (!board[x_pos][y_pos].getChessPiece()->isValidMove(
                x_pos, y_pos, x_dest, y_dest)) {

          if (kingIndex[0] - checkThreat_x == 0) {
            if ((x_dest == kingIndex[0])
                && (((kingIndex[1] - y_dest < 0)
                && (checkThreat_y > y_dest))
                || ((kingIndex[1] - y_dest > 0)
                && (checkThreat_y < y_dest))) ) {
                return true;
                }
          } else if (kingIndex[1] - checkThreat_y == 0) {
            if ((y_dest == kingIndex[1])
               && (((kingIndex[0] - x_dest < 0) && (checkThreat_x - x_dest > 0))
               || ((kingIndex[0] - x_dest > 0) && (checkThreat_x - x_dest < 0)))) {
               return true;
               }
          } else if (abs(kingIndex[1] - checkThreat_y) == abs(kingIndex[0] - checkThreat_x)){
            if (!(((x_dest > kingIndex[0]) && (x_dest > checkThreat_x))
                || ((x_dest < kingIndex[0]) && (x_dest < checkThreat_x)))
                && !(((y_dest > kingIndex[1]) && (y_dest > checkThreat_y))
                || ((y_dest < kingIndex[1]) && (y_dest < checkThreat_y)))
                && abs(x_dest - kingIndex[0]) == abs(y_dest - kingIndex[1])
                && abs(x_dest - checkThreat_x) == abs(y_dest - checkThreat_y))  {
                return true;
                }
          }
      }
    }
  }
  return false;
}

bool ChessBoard::canCaptureThreat(int x_pos, int y_pos) {
  if (board[x_pos][y_pos].getChessPiece()->getType() != "KING") {
    std::string COLOUR = board[checkThreat_x][checkThreat_y].getChessPiece()->getColour();
    return (board[x_pos][y_pos].getChessPiece()->isValidCapture(
        x_pos, y_pos, checkThreat_x, checkThreat_y, COLOUR));
  }
  return false;
}

bool ChessBoard::kingCanEscape(int* kingIndex, std::string COLOUR)
{
  for (int i=-1; i<2; ++i) {
    for (int j=-1; j<2; ++j) {

      int x = kingIndex[0]+i;
      int y = kingIndex[1]+j;

      if (isMoveLegal(kingIndex[0], kingIndex[1], x, y)) {
        if (!isDefended(x, y, COLOUR)) {
          return true;
        }
      }
    }
  }
  return false;
}

bool ChessBoard::isSquareDefended(int x,int y,std::string COLOUR)
{
  for (int x_pos=0;x_pos<8;++x_pos) {
    for (int y_pos=0;y_pos<8;++y_pos) {
      if (board[x_pos][y_pos].isOccupied()
          && board[x_pos][y_pos].getChessPiece()->getColour() != COLOUR) {
        if (!isPathBlocked(x_pos, y_pos, x, y)) {
          if (board[x_pos][y_pos].getChessPiece()->
              isValidCapture(x_pos, y_pos, x, y, COLOUR))
            return true;
        }
      }
    }
  }
  return false;
}

bool ChessBoard::isPathBlocked(int x_pos,int y_pos,int x_dest,int y_dest)
{
  int x_step = (x_pos - x_dest) == 0 ? 0 : (x_pos - x_dest)/abs(x_pos - x_dest);
  int y_step = (y_pos - y_dest) == 0 ? 0 : (y_pos - y_dest)/abs(y_pos - y_dest);

  while (abs(x_dest - x_pos) != 1 && abs(y_dest - y_pos) != 1) {
    x_pos -= x_step;
    y_pos -= y_step;
    if (board[x_pos][y_pos].isOccupied()) {
      return true;
    }
  }
  return false;
}

bool ChessBoard::isMoveLegal(int x_pos,int y_pos,int x_dest,int y_dest)
{
  if (board[x_pos][y_pos].isOccupied()) {

    if (!((x_pos == x_dest) & (y_pos == y_dest))) {

      if (validGridCoords(x_pos, y_pos, x_dest, y_dest)) {

        if (!movedIntoCheck(x_pos, y_pos, x_dest, y_dest)) {

          if (doesMoveResolveCheck(x_pos, y_pos, x_dest, y_dest)) {

            if (board[x_dest][y_dest].isOccupied()) {
              //captures own colour
              if (board[x_pos][y_pos].getChessPiece()->getColour()
                  == board[x_dest][y_dest].getChessPiece()->getColour()) {
                return false;
              }
            }
            //is there another chess piece in the way
            if (!isPathBlocked(x_pos, y_pos, x_dest, y_dest)) {
              //move rules of specific chess piece
              if (board[x_dest][y_dest].isOccupied()) {
                std::string COLOUR = board[x_dest][y_dest].getChessPiece()->getColour();
                return (board[x_pos][y_pos].getChessPiece()->isValidCapture(
                                                                         x_pos, y_pos,
                                                                         x_dest, y_dest,
                                                                         COLOUR));
              } else {
                return (board[x_pos][y_pos].getChessPiece()->isValidMove(
                                                                         x_pos,y_pos,
                                                                         x_dest,y_dest));
              }
            }
          }
        }
      }
    }
  }
  return false;
}

void ChessBoard::setPromotingColour(std::string COLOUR)
{
  promotingColour = COLOUR;
}

void ChessBoard::setPromotingPiece(std::string type)
{
  promotingPiece = type;
}

std::string ChessBoard::getPromotingColour()
{
  return promotingColour;
}

std::string ChessBoard::getPromotingPiece()
{
  return promotingPiece;
}

ChessPiecePtr ChessBoard::getChessPiece(int x, int y)
{
  return board[x][y].getChessPiece();
}

std::string ChessBoard::getType(int x, int y)
{
  return board[x][y].getChessPiece()->getType();
}

std::string ChessBoard::getColour(int x, int y)
{
  return board[x][y].getChessPiece()->getColour();
}

bool ChessBoard::isOccupied(int x_pos, int y_pos)
{
  return board[x_pos][y_pos].isOccupied();
}

bool ChessBoard::validGridCoords(int x_pos, int y_pos, int x_dest, int y_dest)
{
  return (x_pos >= 0 && x_pos < 8 && y_pos >= 0 && y_pos < 8) &&
      (x_dest >= 0 && x_dest < 8 && y_dest >= 0 && y_dest < 8);
}

bool ChessBoard::onStraightLine(int x_pos, int y_pos, int x_dest, int y_dest)
{
  return ((x_pos == kingIndex[0]) || (y_pos == kingIndex[1]) ||
          (abs(y_pos - kingIndex[1]) && abs(x_pos - kingIndex[0]))) &&
          !((x_pos == kingIndex[0]) && (y_pos == kingIndex[1]));
}

bool ChessBoard::isDefended(int x, int y, std::string COLOUR)
{
  for (int x_pos=0;x_pos<8;x_pos++) {
    for (int y_pos=0;y_pos<8;y_pos++) {
      if (board[x_pos][y_pos].isOccupied()) {
        if (!isPathBlocked(x_pos, y_pos, x, y)) {
          if (board[x_pos][y_pos].getChessPiece()->isDefending(x_pos, y_pos, x, y, COLOUR)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

void ChessBoard::reset()
{
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      board[i][j].setChessPiece(NULL);
    }
  }
  //white pieces constructed
  for (int i = 0; i < 8; ++i) {
    board[6][i] = Square(ChessPiecePtr(new Pawn("WHITE","PAWN")));
  }
  board[7][0] = Square(ChessPiecePtr(new Rook("WHITE","ROOK")),true);
  board[7][7] = Square(ChessPiecePtr(new Rook("WHITE","ROOK")),true);
  board[7][1] = Square(ChessPiecePtr(new Knight("WHITE","KNIGHT")),true);
  board[7][6] = Square(ChessPiecePtr(new Knight("WHITE","KNIGHT")),true);
  board[7][2] = Square(ChessPiecePtr(new Bishop("WHITE","BISHOP")),true);
  board[7][5] = Square(ChessPiecePtr(new Bishop("WHITE","BISHOP")),true);
  board[7][3] = Square(ChessPiecePtr(new Queen("WHITE","QUEEN")),true);
  board[7][4] = Square(ChessPiecePtr(new King("WHITE","KING")),true);

  //Black pieces constructed
  for (int i=0;i<8;++i) {
    board[1][i] = Square(ChessPiecePtr(new Pawn("BLACK","PAWN")));
  }
  board[0][0] = Square(ChessPiecePtr(new Rook("BLACK","ROOK")),true);
  board[0][7] = Square(ChessPiecePtr(new Rook("BLACK","ROOK")),true);
  board[0][1] = Square(ChessPiecePtr(new Knight("BLACK","KNIGHT")),true);
  board[0][6] = Square(ChessPiecePtr(new Knight("BLACK","KNIGHT")),true);
  board[0][2] = Square(ChessPiecePtr(new Bishop("BLACK","BISHOP")),true);
  board[0][5] = Square(ChessPiecePtr(new Bishop("BLACK","BISHOP")),true);
  board[0][3] = Square(ChessPiecePtr(new Queen("BLACK","QUEEN")),true);
  board[0][4] = Square(ChessPiecePtr(new King("BLACK","KING")),true);

  mostRecentMove[0] = -1;
  mostRecentMove[1] = -1;

  std::string promotingColour = "WHITE";
  std::string promotingPiece = "QUEEN";
  kingIndex[0] = 7;
  kingIndex[1] = 4;

  checkStatus = CheckStatus::NotInCheck;
}

void ChessBoard::testForStalemate(std::string PLAYER_TURN)
{
  std::string OPPONENT_COLOUR = (PLAYER_TURN == "WHITE" ? "BLACK" : "WHITE");
  findKing(OPPONENT_COLOUR);
  int legalMoves = 0;
  int matingPieces = 0;
  int pieces = 0;
  for (int x_pos = 0; x_pos < 8; ++x_pos) {
    for (int y_pos = 0; y_pos < 8; ++y_pos) {
      if (board[x_pos][y_pos].isOccupied()) {
        if (board[x_pos][y_pos].getChessPiece()->matingPiece()) {
          matingPieces++;
        }
        pieces++;
        if (board[x_pos][y_pos].getChessPiece()->getColour() == OPPONENT_COLOUR) {
        std::vector<std::array<int, 2>> possibleMoves = board[x_pos][y_pos].getChessPiece()->possibleMoves(x_pos, y_pos);
        for (const auto & elem : possibleMoves) {
          if (isMoveLegal(x_pos, y_pos, elem[0], elem[1])) {
            legalMoves++;
          }
        }
        }
      }
    }
  }
  if (legalMoves == 0) {
    moveType = MoveType::StaleMate;
  } else if (pieces == 2) {
    moveType = MoveType::Draw;
  } else if (pieces == 3) {
      if (matingPieces == 0) {
        moveType = MoveType::Draw;
      }
  }
}
