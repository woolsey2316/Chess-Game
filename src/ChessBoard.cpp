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
#include "PromotionScreen.h"

#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <memory>
#include <iostream>

typedef std::shared_ptr<ChessPiece> ChessPiecePtr;

MoveType ChessBoard::playerMove(std::string playerTurn, int x_pos, int y_pos,
								int x_dest, int y_dest, char promoType)
{
  moveType = MoveType::Invalid;
  if (!validGridCoords(x_pos, y_pos, x_dest, y_dest)) {
    return moveType;
  }
  //user didn't select a piece to move, it was an empty square
  if (!board[x_pos][y_pos].isOccupied()){
    return moveType;
  }
  //the piece didn't even move
  if (x_pos == x_dest && y_pos == y_dest) {
    return moveType;
  }
  //checks if it is the player's turn or alternatively moved your own piece
  if (getChessPieceColour(x_pos,y_pos) != playerTurn){
    return moveType;
  }
  //update configuration of board, if move is invalid unwind the move later
  updateBoard(x_pos, y_pos, x_dest, y_dest);

  if (promoType == 'q'|| promoType == 'b' || promoType == 'k' || promoType == 'r') {
    std::string pieceType;
    setPromotingColour(playerTurn);
    if (promoType == 'q') {
      pieceType = "queen";
    } else if (promoType == 'b') {
      pieceType = "bishop";
    } else if (promoType == 'r') {
      pieceType = "rook";
    } else if (promoType == 'k') {
      pieceType = "knight";
    }
    setPromotingPiece(pieceType);
    board[x_dest][y_dest].promotePawn(pieceType, playerTurn);
    return MoveType::Promotion;
  }

  std::string chessPieceType = getChessPieceType(x_dest,y_dest);
  bool opponentKing = false;
  //determine if your own king is in check
  determineCheckStatus(playerTurn, opponentKing);

  //A move that involves a capture
  if (capturedChesspiece!=NULL) {
    //check if piece captures the other colour
    moveType = captureChessPiece(x_pos, y_pos, x_dest, y_dest);
    if (moveType == MoveType::Invalid) {
      if (isValidCapture(x_pos, y_pos, x_dest, y_dest, playerTurn, true)) {
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
      enPassantCapture = (x_dest - x_pos == 2) ? true : false;
    }
    enPassantCapture = false;

    opponentKing = true;

    if (determineCheckStatus(playerTurn, opponentKing) == CheckStatus::CheckMate) {
      return MoveType::CheckMate;
      std::cout << "Check Mate" << playerTurn << "Wins!" << std::endl;
    }
    if (checkStatus == CheckStatus::NotInCheck) {
      testForStalemate(playerTurn);
    }
    return moveType;
  }
  reverseMove(x_pos, y_pos, x_dest, y_dest);


  return moveType;
}
/*
Assume any move is legal and tentatively update the board's configuration
we then reverse the move if the configuration of the board is invalid
*/
void ChessBoard::updateBoard(int x_pos, int y_pos, int x_dest, int y_dest)
{
  if (x_pos == x_dest && y_pos == y_dest) {
    return;
  }

  chesspieceTouched = board[x_pos][y_pos].getChessPiece();
  capturedChesspiece = board[x_dest][y_dest].getChessPiece();

    if (capturedChesspiece == NULL
        && getChessPieceType(x_pos,y_pos) == "PAWN"
        && abs(x_pos - x_dest) == 1
        && abs(y_pos - y_dest) == 1) {
            //enPassant move captures adjacent pawn
            setEnPassant(true);
            capturedChesspieceByEnPassant = board[x_pos][y_dest].getChessPiece();
            board[x_pos][y_dest].setChessPiece(NULL);

        } else {
          setEnPassant(false);
        }

  board[x_dest][y_dest].setChessPiece(chesspieceTouched);
  board[x_pos][y_pos].setChessPiece(NULL);
}

bool ChessBoard::doesMoveResolveCheck(int x_pos, int y_pos, int x_dest, int y_dest,
                                      std::array<size_t,2> checkThreat,
                                      std::array<size_t,2> kingIndex)
{
  //never in check
  if (checkThreat[0] == 9) {
    return true;
  }
  std::string PLAYER_TURN = getChessPieceColour(x_dest,y_dest);
  if (checkStatus == CheckStatus::NotInCheck) {
    return true;
  } else if (checkStatus == CheckStatus::Check) {
    if (kingIndex[0] == x_dest && kingIndex[1] == y_dest) {
      return !isSquareDefended(x_dest, y_dest, PLAYER_TURN);
    }
    if (isPinnedToKing(x_dest, y_dest, kingIndex, PLAYER_TURN)) {
        return false;
    }
    return (canBlockorCaptureThreat(x_pos, y_pos, x_dest, y_dest,
									checkThreat, kingIndex, true));
  } else if (checkStatus == CheckStatus::DoubleCheck) {
    //must move king when in double check
    if (getChessPieceType(x_dest,y_dest) == "KING") {
      if (isSquareDefended(x_dest, y_dest, PLAYER_TURN)) {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

bool ChessBoard::isPinnedToKing(int x_pos, int y_pos,
								std::array<size_t,2> kingIndex,
								std::string pinned_piece_colour)
{
  int x_pinned = x_pos; int y_pinned = y_pos;
  if (x_pos == kingIndex[0] && y_pos == kingIndex[1]) {
    return false;
  }
  if (!onStraightLine(x_pos, y_pos, kingIndex[0], kingIndex[1], kingIndex)) {
    return false;
  }
  if (isPathBlocked(x_pos, y_pos, kingIndex[0], kingIndex[1])) {
    return false;
  }
  int x_step = (x_pos - kingIndex[0]) == 0 ? 0 : (x_pos - kingIndex[0])/abs(x_pos - kingIndex[0]);
  int y_step = (y_pos - kingIndex[1]) == 0 ? 0 : (y_pos - kingIndex[1])/abs(y_pos - kingIndex[1]);

  //searching for a pinning piece
  while (x_pos < 7 && x_pos > 0 && y_pos < 7 && y_pos > 0) {
    x_pos += x_step;
    y_pos += y_step;
    if (board[x_pos][y_pos].isOccupied()) {
      //a piece between threat and king exists
      if (!isPathBlocked(x_pos, y_pos, x_pinned, y_pinned)) {
        if (getChessPieceType(x_pos,y_pos) != "PAWN" &&
            getChessPieceType(x_pos,y_pos) != "KNIGHT") {
          if (isDefending(x_pos, y_pos, x_pinned, y_pinned, pinned_piece_colour)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool ChessBoard::breaksThePin(int x_pos, int y_pos, int x_dest, int y_dest,
                              std::array<size_t,2> kingIndex)
{
  return !onStraightLine(x_pos, y_pos, x_dest, y_dest, kingIndex);
}

MoveType ChessBoard::promotePawn(int x_pos, int y_pos, int x_dest, int y_dest)
{
  std::string COLOUR = getChessPieceColour(x_dest,y_dest);
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
  setPromotingColour(getChessPieceColour(x_dest,y_dest));
  setPromotingPiece(pieceType);
  return MoveType::Promotion;
}

MoveType ChessBoard::moveToEmptySquare(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way
  //and whether the piece moved validly according its own move rules
  std::string COLOUR = getChessPieceColour(x_dest,y_dest);

  std::array<size_t,2> kingIndex = findKing(COLOUR);
  std::array<size_t,2> checkThreat = findCheckThreat(COLOUR, kingIndex);
  if (isMoveLegal(x_pos, y_pos, x_dest, y_dest, checkThreat, kingIndex)) {
    return MoveType::EmptySquare;
  }
  return MoveType::Invalid;
}

MoveType ChessBoard::enPassant(int x_pos, int y_pos, int x_dest, int y_dest)
{
  if (enPassantCapture) {
    if (board[x_pos][y_dest].isOccupied()) {
      if (isValidEnPassant(x_pos, y_pos, x_dest, y_dest,"PAWN", mostRecentMove)) {
        return MoveType::enPassant;
      }
    }
  }
  return MoveType::Invalid;
}

bool ChessBoard::kingAttemptingCastle(int x_pos, int y_pos, int x_dest, int y_dest)
{
  return (getChessPieceType(x_dest,y_dest) == "KING"
          && x_pos-x_dest == 0
          && abs(y_dest - y_pos) == 2)
          && (!board[x_dest][y_dest].getChessPiece()->moved());
}

MoveType ChessBoard::castle(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way and whether
  //the piece moved validly according its own move rules
  std::string COLOUR = getChessPieceColour(x_dest,y_dest);
  if (isLegalCastle(x_pos, y_pos, x_dest, y_dest, COLOUR)) {
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
  for (int i = 0; i < 2; ++i) {
    y_pos += moveRightorLeft;
    if (isDefended(x_pos, y_pos, COLOUR))
      return false;
  }
  return true;
}

MoveType ChessBoard::captureChessPiece(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way and whether
  //the piece moved validly according its own move rules
  std::string COLOUR = getChessPieceColour(x_dest,y_dest);
  std::string ENEMY_COLOUR = (COLOUR == "WHITE" ? "BLACK" : "WHITE");

  std::array<size_t,2> kingIndex = findKing(COLOUR);
  std::array<size_t,2> checkThreat = findCheckThreat(ENEMY_COLOUR, kingIndex);

  if (isMoveLegal(x_pos, y_pos, x_dest, y_dest, checkThreat, kingIndex)) {
      return MoveType::Capture;
  }
  return MoveType::Invalid;
}
//Determines if the white or black king is in check or if the opponent king
//is checkmated
CheckStatus ChessBoard::determineCheckStatus(std::string COLOUR, bool opponentKing)
{
  std::string OPPONENT_COLOUR;
  if (opponentKing) {
    COLOUR = (COLOUR == "WHITE" ? "BLACK" : "WHITE");
  }
  OPPONENT_COLOUR = (COLOUR == "WHITE" ? "BLACK" : "WHITE");

  std::array<size_t,2> kingIndex = findKing(COLOUR);
  std::array<size_t,2> checkThreat;
  //find if the chessboard is in check and the threats
  checkStatus = CheckStatus::NotInCheck;
  //iterate through whole chessboard to see if any piece can capture the king
  for (int x=0;x<8;++x) {
    for (int y=0;y<8;++y) {
      if (board[x][y].isOccupied() && getChessPieceColour(x,y) != COLOUR) {
        if (!isPathBlocked(x, y, kingIndex[0], kingIndex[1]))
          if (isValidCapture(x, y, kingIndex[0], kingIndex[1], COLOUR, false)) {
            if (checkStatus == CheckStatus::NotInCheck){
              checkStatus = CheckStatus::Check;
            }
            else {
              checkStatus = CheckStatus::DoubleCheck;
            }
            checkThreat[0] = x;
            checkThreat[1] = y;
          }
      }
    }
  }

  if (!opponentKing) {
    //you don't move into CheckMate, finish method here
    return checkStatus;
  }

  if (checkStatus == CheckStatus::Check) {
    if(!kingCanEscape(kingIndex, COLOUR)) {
      for (int x_pos=0;x_pos<8;++x_pos) {
        for (int y_pos=0;y_pos<8;++y_pos) {
          if (board[x_pos][y_pos].isOccupied() &&
              getChessPieceColour(x_pos,y_pos) == COLOUR) {

              std::vector<std::array<int, 2>> possibleMoves = findPossibleMoves(x_pos, y_pos);
              for (const auto & dest : possibleMoves) {
                if (canBlockorCaptureThreat(x_pos, y_pos, dest[0], dest[1],
                                            checkThreat, kingIndex, false)) {
                  return CheckStatus::Check;
                }
              }
          }
        }
      }
      std::cout << "Check Mate" << std::endl;
      return CheckStatus::CheckMate;
    }
  } else if (checkStatus == CheckStatus::DoubleCheck) {
    if (!kingCanEscape(kingIndex, COLOUR)) {
      return CheckStatus::CheckMate;
    }
  }
  return checkStatus;

}

std::array<size_t,2> ChessBoard::findKing(std::string COLOUR)
{
  std::array<size_t,2> kingIndex;
  for (int i=0;i<8;++i) {
    for (int j=0;j<8;++j) {
      if (board[i][j].isOccupied()) {
        if (board[i][j].getChessPiece()->getType() == "KING"
            && (getChessPieceColour(i,j)==COLOUR)){
              kingIndex[0] = i;
              kingIndex[1] = j;
              return kingIndex;
        }
      }
    }
  }
}

bool ChessBoard::movedIntoCheck(int x_pos, int y_pos, int x_dest, int y_dest,
                                std::array<size_t,2> kingIndex)
{
  std::string COLOUR = getChessPieceColour(x_dest,y_dest);
  if (x_dest == kingIndex[0] && y_dest == kingIndex[1]) {
    return isSquareDefended(x_dest, y_dest, COLOUR);
  }

  for (int x=0;x<8;++x) {
    for (int y=0;y<8;++y) {
      if (board[x][y].isOccupied() && getChessPieceColour(x,y) != COLOUR) {
        if (!isPathBlocked(x, y, kingIndex[0], kingIndex[1]))
          if (isValidCapture(kingIndex[0], kingIndex[1], x, y, COLOUR, true)) {
              return true;
          }
      }
    }
  }

  if (isPinnedToKing(x_pos, y_pos, kingIndex, COLOUR)) {
    if (breaksThePin(x_pos, y_pos, x_dest, y_dest, kingIndex)) {
      return true;
    }
  }
  return false;
}

//finds the position of a piece that attacks its opponents' king
std::array<size_t,2> ChessBoard::findCheckThreat(std::string COLOUR,
                                     std::array<size_t,2> kingIndex)
{
  std::array<size_t,2> checkThreat;
  for (int x=0;x<8;++x) {
    for (int y=0;y<8;++y) {
      if (board[x][y].isOccupied() && getChessPieceColour(x,y) != COLOUR) {
        if (!isPathBlocked(x, y, kingIndex[0], kingIndex[1]))
          if (isValidCapture(x, y, kingIndex[0], kingIndex[1], COLOUR, false)) {
            checkThreat[0] = x;
            checkThreat[1] = y;
            return checkThreat;
          }
      }
    }
  }
}
/*
	Can a piece place itself between the king and its attacker, or better yet
	capture it.
*/
bool ChessBoard::canBlockorCaptureThreat(int x_pos, int y_pos, int x_dest,
                                         int y_dest,
                                         std::array<size_t,2> checkThreat,
                                         std::array<size_t,2> kingIndex,
                                         bool alreadyMoved)
{
    //3 cases diagonal threat, horizontal threat, vertical threat
    std::string COLOUR = getChessPieceColour(checkThreat[0],checkThreat[1]);
    std::string CAPTURING_COLOUR = getChessPieceColour(x_pos,y_pos);
    std::string PIECE_TYPE = getChessPieceType(checkThreat[0],checkThreat[1]);
    if (getChessPieceType(x_pos,y_pos) != "KING") {
      if (enPassantCapture) {
        if (board[x_pos][y_pos].getChessPiece()->isValidEnPassant(
          x_pos, y_pos, checkThreat[0], y_pos, PIECE_TYPE, mostRecentMove)) {
          return true;
        }
      }
      if (!isPathBlocked(x_pos, y_pos, checkThreat[0], checkThreat[1])) {
          if (!isPinnedToKing(x_pos, y_pos, kingIndex, CAPTURING_COLOUR)) {
            if (isValidCapture(x_pos, y_pos, checkThreat[0], checkThreat[1], COLOUR, alreadyMoved)) {
              return true;
            }
            // checks if three pieces on straight line
            if (kingIndex[0] == checkThreat[0] && x_dest == kingIndex[0]) {
              return isBetween(kingIndex[1], y_dest, checkThreat[1]);
            } else if (kingIndex[1] == checkThreat[1] && y_dest == kingIndex[1]) {
              return isBetween(kingIndex[0], x_dest, checkThreat[0]);
            } else if (abs(kingIndex[1] - checkThreat[1]) == abs(kingIndex[0] - checkThreat[0])
              && abs(x_dest - kingIndex[0]) == abs(y_dest - kingIndex[1])) {
              return isBetween(kingIndex[1], y_dest, checkThreat[1]) &&
              isBetween(kingIndex[0], x_dest, checkThreat[0]);
            }
          }
        }
    }
  return false;
}

bool ChessBoard::isBetween(int pos1, int mid, int pos2)
{
	return (((pos1 < mid ) && (pos2 > mid)) ||
           ((pos1 > mid ) && (pos2 < mid)));
}

bool ChessBoard::kingCanEscape(std::array<size_t,2> kingIndex,
                               std::string COLOUR)
{
  std::vector<std::array<int, 2>> possibleMoves =
		findPossibleMoves(kingIndex[0], kingIndex[1]);
  for (const auto & elem : possibleMoves) {
    int x = elem[0];
    int y = elem[1];
    ChessPiecePtr theKing;
    ChessPiecePtr attackedByKing;
    //make hypothetical king move to see if king thereby escapes check
    theKing = board[kingIndex[0]][kingIndex[1]].getChessPiece();
    attackedByKing = board[x][y].getChessPiece();
    board[kingIndex[0]][kingIndex[1]].setChessPiece(NULL);
    board[x][y].setChessPiece(theKing);

    //did the hypothetical king move invlove capturing a piece
    if (attackedByKing != NULL) {
      std::string COLOUR = theKing->getColour();
      if (isValidCapture(kingIndex[0], kingIndex[1], x, y, attackedByKing->getColour(),true)) {
        if (!isDefended(x, y, COLOUR)) {
          //reverse hypothetical move
          board[x][y].setChessPiece(attackedByKing);
          board[kingIndex[0]][kingIndex[1]].setChessPiece(theKing);
          return true;
        }
      }
    } else {
      if (isValidMove(kingIndex[0], kingIndex[1], x, y, true)) {
        if (!isDefended(x, y, COLOUR)) {
          //reverse hypothetical move
          board[x][y].setChessPiece(attackedByKing);
          board[kingIndex[0]][kingIndex[1]].setChessPiece(theKing);
          return true;
        }
      }
    }
    //reverse hypothetical move
    board[x][y].setChessPiece(attackedByKing);
    board[kingIndex[0]][kingIndex[1]].setChessPiece(theKing);
  }
  return false;
}

bool ChessBoard::isSquareDefended(int x,int y,std::string COLOUR)
{
  for (int x_pos=0;x_pos<8;++x_pos) {
    for (int y_pos=0;y_pos<8;++y_pos) {
      if (board[x_pos][y_pos].isOccupied()
          && getChessPieceColour(x_pos,y_pos) != COLOUR) {
        if (!isPathBlocked(x_pos, y_pos, x, y)) {
          if (isValidCapture(x_pos, y_pos, x, y, COLOUR, false))
            return true;
        }
      }
    }
  }
  return false;
}

bool ChessBoard::isPathBlocked(int x_pos,int y_pos,int x_dest,int y_dest)
{
  if (x_pos == x_dest && y_pos == y_dest) {
    return false;
  }

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

bool ChessBoard::isMoveLegal(int x_pos,int y_pos,int x_dest,int y_dest,
                             std::array<size_t,2> checkThreat,
                             std::array<size_t,2> kingIndex)
{
  if (board[x_dest][y_dest].isOccupied()) {

    if (!((x_pos == x_dest) & (y_pos == y_dest))) {

      if (validGridCoords(x_pos, y_pos, x_dest, y_dest)) {

        if (!movedIntoCheck(x_pos, y_pos, x_dest, y_dest, kingIndex)) {

          if (doesMoveResolveCheck(x_pos, y_pos, x_dest, y_dest, checkThreat, kingIndex)) {

            if (capturedChesspiece != NULL) {
              //captures own colour
              if (getChessPieceColour(x_dest,y_dest) == capturedChesspiece->getColour()) {
                return false;
              }
            }
            //is there another chess piece in the way
            if (!isPathBlocked(x_pos, y_pos, x_dest, y_dest)) {
              //move rules specific to chess piece
              if (capturedChesspiece != NULL) {
                std::string COLOUR = capturedChesspiece->getColour();
                return (isValidCapture(x_pos,y_pos,x_dest, y_dest,COLOUR,true));
              } else {
                return (isValidMove(x_pos,y_pos,x_dest,y_dest,true));
              }
            }
          }
        }
      }
    }
  }
  return false;
}

bool ChessBoard::validGridCoords(int x_pos, int y_pos, int x_dest, int y_dest)
{
  return (x_pos >= 0 && x_pos < 8 && y_pos >= 0 && y_pos < 8) &&
      (x_dest >= 0 && x_dest < 8 && y_dest >= 0 && y_dest < 8);
}

bool ChessBoard::onStraightLine(int x_pos, int y_pos, int x_dest, int y_dest,
                                std::array<size_t,2> kingIndex)
{
  //The direction a piece has to travel to reach the king
  int x_step = (x_pos - kingIndex[0]) == 0 ? 0 : (x_pos - kingIndex[0])/abs(x_pos - kingIndex[0]);
  int y_step = (y_pos - kingIndex[1]) == 0 ? 0 : (y_pos - kingIndex[1])/abs(y_pos - kingIndex[1]);

  //checks if you encounter the king on your steps
  while (x_pos < 7 && x_pos > 0 && y_pos < 7 && y_pos > 0) {
    x_pos -= x_step;
    y_pos -= y_step;
    if (x_pos == x_dest && y_pos == y_dest) {
      return true;
    }
  }

  //may not need to reverse direction
  do {
    x_pos += x_step;
    y_pos += y_step;
    if (x_pos == x_dest && y_pos == y_dest) {
      return true;
    }
  } while (x_pos < 7 && x_pos > 0 && y_pos < 7 && y_pos > 0);

  return false;
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

  checkStatus = CheckStatus::NotInCheck;
}

void ChessBoard::testForStalemate(std::string PLAYER_TURN)
{
  std::string OPPONENT_COLOUR = (PLAYER_TURN == "WHITE" ? "BLACK" : "WHITE");
  std::array<size_t,2> kingIndex = findKing(OPPONENT_COLOUR);
  std::array<size_t,2> debug = findKing(OPPONENT_COLOUR);
  std::array<size_t,2> checkThreat = {9,9};
  std::vector<std::array<int, 2>> possibleMoves;
  bool modifyingKingPosition = false;
  int legalMoves = 0;
  int numChessPieces = 0;
  int neccesaryMatingPieces = 0;
  int numKnights;
  for (int x_pos = 0; x_pos < 8; ++x_pos) {
    for (int y_pos = 0; y_pos < 8; ++y_pos) {
      if (board[x_pos][y_pos].isOccupied()) {
        if (board[x_pos][y_pos].getChessPiece()->matingPiece()) {
          neccesaryMatingPieces++;
        }
        if (getChessPieceType(x_pos, y_pos) == "KNIGHT") {
          numKnights++;
        }
        numChessPieces++;

        if (getChessPieceColour(x_pos,y_pos) == OPPONENT_COLOUR) {
            possibleMoves.clear();
            possibleMoves = findPossibleMoves(x_pos, y_pos);
          if (!possibleMoves.empty())  {
            for (const auto elem : possibleMoves) {
              updateBoard(x_pos, y_pos, elem[0], elem[1]);
              //modified king position
              if (x_pos == kingIndex[0] && y_pos == kingIndex[1]) {
                kingIndex[0] = elem[0]; kingIndex[1] = elem[1];
                modifyingKingPosition = true;
                if (kingCanEscape(kingIndex,OPPONENT_COLOUR)) {
                  legalMoves++;
                }
              } else {
                if (isMoveLegal(x_pos,y_pos,elem[0],elem[1],
                                 checkThreat, kingIndex)) {
                  legalMoves++;
                }
              }
              reverseMove(x_pos, y_pos, elem[0], elem[1]);
              if (modifyingKingPosition) {
                kingIndex[0] = x_pos; kingIndex[1] = y_pos;
                modifyingKingPosition = false;
                break;
              }
            }
          }
        }
      }
    }
  }
  if (legalMoves == 0) {
    moveType = MoveType::StaleMate;
  } else if (neccesaryMatingPieces == 0) {
    if (numChessPieces == 3 && numKnights < 2) {
      moveType = MoveType::Draw;
    } else if (numChessPieces == 2) {
      moveType = MoveType::Draw;
    }
  }
}

ChessBoard::ChessBoard()
{
  reset();
}

void ChessBoard::reverseMove(int x_pos, int y_pos, int x_dest, int y_dest)
{
  board[x_pos][y_pos].setChessPiece(chesspieceTouched);
  board[x_dest][y_dest].setChessPiece(capturedChesspiece);
  if (enPassantCapture) {
    board[x_pos][y_dest].setChessPiece(capturedChesspieceByEnPassant);
  }
  setEnPassant(false);
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

std::string ChessBoard::getChessPieceType(int x_pos,int y_pos)
{
  return board[x_pos][y_pos].getChessPiece()->getType();
}

std::string ChessBoard::getChessPieceColour(int x_pos,int y_pos)
{
	return board[x_pos][y_pos].getChessPiece()->getColour();
}

bool ChessBoard::isValidCapture(int x_pos,int y_pos,int x_dest,
                                int y_dest,std::string COLOUR,
                                bool alreadyMoved)
{
  if (alreadyMoved) {
    return board[x_dest][y_dest].getChessPiece()->isValidCapture(x_pos,y_pos,
                                                    x_dest,y_dest,COLOUR);
  } else {
    return board[x_pos][y_pos].getChessPiece()->isValidCapture(x_pos,y_pos,
                                                    x_dest,y_dest,COLOUR);
  }
}

bool ChessBoard::isValidMove(int x_pos,int y_pos,int x_dest,int y_dest,
                             bool alreadyMoved)
{
  if (alreadyMoved) {
    return board[x_dest][y_dest].getChessPiece()->isValidMove(x_pos,y_pos,
                                                              x_dest,y_dest);
  } else {
    return board[x_pos][y_pos].getChessPiece()->isValidMove(x_pos,y_pos,
                                                            x_dest,y_dest);
  }

}

bool ChessBoard::isDefending(int x_pos,int y_pos,int x,
                             int y,std::string pinned_piece_colour)
{
	return board[x_pos][y_pos].getChessPiece()->isDefending(x_pos, y_pos, x, y,
                                                          pinned_piece_colour);
}

std::vector<std::array<int, 2>> ChessBoard::findPossibleMoves(int x_pos,int y_pos)
{
	return board[x_pos][y_pos].getChessPiece()->possibleMoves(x_pos,y_pos);
}

bool ChessBoard::isValidEnPassant(int x_pos,int y_pos,int x_dest,int y_dest,
                                  std::string pieceType,int* mostRecentMove)
{
	return board[x_dest][y_dest].getChessPiece()->isValidEnPassant(
		x_pos, y_pos, x_dest, y_dest, pieceType, mostRecentMove);
}

void ChessBoard::setEnPassant(bool twoStepMove)
{
  enPassantCapture = twoStepMove;
}

