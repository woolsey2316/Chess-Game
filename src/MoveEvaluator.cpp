#include "MoveEvaluator.h"
#include <string>

MoveType determineMoveType(std::string playerTurn, int x_pos, int y_pos, int x_dest, int y_dest, ChessBoard chessboard)
{
  MoveType moveType = MoveType::Invalid;
  chessboard.getChessPiece(x_pos, y_pos);
  //keep a copy of chesspieces in-case of reversing a move
  ChessPiecePtr chessPiece1 = chessboard.getChessPiece(x_pos, y_pos);
  ChessPiecePtr chessPiece2 = chessboard.getChessPiece(x_dest, y_dest);
  std::string chessPieceType;
  //user didn't select a piece to move was an empty square
  if (!chessboard.isOccupied(x_pos,y_pos)){
    return moveType;
  } else {
    chessPieceType = chessPiece1->getType();
  }
  //didn't even move
  if (x_pos == x_dest && y_pos == y_dest) {
    return moveType;
  }
  //checks if it is the player's turn
  if (chessboard.getColour(x_pos, y_pos) != playerTurn){
    return moveType;
  }


    //A move that involves a capture or castling
    if (chessboard.isOccupied(x_dest, y_dest)) {
        //check if piece captures the other colour
        moveType = captureChessPiece(x_pos, y_pos, x_dest, y_dest);
        if (moveType == MoveType::Invalid) {
          //Pawn has its own capture method
          if (chessboard.getType(x_pos,y_pos) == "PAWN"){
            if (validPawnCapture(x_pos, y_pos, x_dest, y_dest, chessboard.getColour(x_pos, y_pos))) {
              moveType = captureChessPiece(x_pos, y_pos, x_dest, y_dest);
            }
          }
        }
    } else if (kingAttemptingCastle(x_pos, y_pos, x_dest, y_dest, !chessPointer1->moved())) {
        moveType = (castle(x_pos, y_pos, x_dest, y_dest));
    } else if (chessPieceType == "PAWN") {
        moveType = enPassant(x_pos, y_pos, x_dest, y_dest);
    }
    if (moveType == MoveType::Invalid) {
      moveType = moveToEmptySquare(x_pos, y_pos, x_dest, y_dest);
    }
    if (moveType != MoveType::Invalid) {
      if (movedIntoCheck(playerTurn)) {
        //reverse the move since player's own king is in check
        std::cout << playerTurn + " You are in Check." << std::endl;
        board[x_pos][y_pos].setChessPiece(chessPointer1);
        board[x_dest][y_dest].setChessPiece(chessPointer2);
        return MoveType::Invalid;
      }

      board[x_dest][y_dest].getChessPiece()->setHasMoved(true);
      board[x_dest][y_dest].getChessPiece()->incrementTotalMoves();
      mostRecentMove[0] = x_dest;
      mostRecentMove[1] = y_dest;

      if (chessPieceType == "PAWN" && board[x_dest][y_dest].canPromotePawn()) {
        moveType = MoveType::Promotion;
        std::string pieceType;
        std::cout << "Enter QUEEN, KNIGHT, BISHOP or ROOK" << std::endl;
        bool correctEntry = false;
        while (!correctEntry) {
          std::cin >> pieceType;
          correctEntry = board[x_dest][y_dest].promotePawn(pieceType,board[x_dest][y_dest].getChessPiece()->getColour());
          if (!correctEntry) {
            std::cout << "Invalid entry, Enter QUEEN, KNIGHT, BISHOP or ROOK" << std::endl;
          }
        }
        setPromotingColour(board[x_dest][y_dest].getChessPiece()->getColour());
        setPromotingPiece(pieceType);
      }
      opponentKing = true;
      if (determineCheckStatus(playerTurn, opponentKing) == CheckStatus::CheckMate) {
        return MoveType::CheckMate;
        std::cout << "Check Mate" << playerTurn << "Wins!" << std::endl;
      }
    }
    return moveType;
}

MoveType MoveEvaluator::captureChessPiece(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way and whether
  //the piece moved validly according its own move rules
  if (isMoveLegal(x_pos, y_pos, x_dest, y_dest)) {
      board[x_dest][y_dest].setChessPiece(board[x_pos][y_pos]);
      return MoveType::Capture;
  }
  return MoveType::Invalid;
}

MoveType MoveEvaluator::moveToEmptySquare(int x_pos, int y_pos, int x_dest, int y_dest)
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

MoveType MoveEvaluator::enPassant(int x_pos, int y_pos, int x_dest, int y_dest)
{
  //checks if the piece moved at all, whether other chesspieces are in its way and whether
  //the piece moved validly according its own move rules
  if (board[x_pos][y_dest].isOccupied()) {
    std::string pieceType = board[x_pos][y_dest].getChessPiece()->getType();
    ChessPiecePtr debug = board[x_pos][y_pos].getChessPiece();
    if (board[x_pos][y_pos].getChessPiece()->isValidEnPassant(x_pos, y_pos, x_dest, y_dest, pieceType, mostRecentMove)) {
      board[x_dest][y_dest].setChessPiece(board[x_pos][y_pos]);
      board[x_pos][y_dest].setChessPiece(NULL);
      return MoveType::enPassant;
    }
  }
  return MoveType::Invalid;
}

bool MoveEvaluator::kingAttemptingCastle(int x_pos, int y_pos, int x_dest, int y_dest, bool notMoved)
{
  return (board[x_pos][y_pos].getChessPiece()->getType() == "KING"
          && x_pos-x_dest==0
          && abs(y_dest - y_pos)==2)
          && (notMoved) ;
}

MoveType MoveEvaluator::castle(int x_pos, int y_pos, int x_dest, int y_dest)
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

bool MoveEvaluator::isLegalCastle(int x_pos,int y_pos,int x_dest,int y_dest,std::string COLOUR)
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
  if (isVulnerableSquare(x_pos, y_pos))
    return false;

  //checks if any square the king passes through is vulnerable to attack
  for (int i=0;i<2;++i) {
    y_pos+=moveRightorLeft;
    if (isVulnerableSquare(x_pos, y_pos, COLOUR))
      return false;
  }
  //castling by queenside requires checking 1 more square
  if (queenSide){
    if (board[x_pos][y_pos-1].isOccupied())
      return false;
  }
  return true;
}

MoveType MoveEvaluator::captureChessPiece(int x_pos, int y_pos, int x_dest, int y_dest)
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
CheckStatus MoveEvaluator::determineCheckStatus(std::string COLOUR, bool opponentKing)
{

  if (opponentKing) {
    COLOUR = (COLOUR == "WHITE" ? "BLACK" : "WHITE");
  }

  //iterate through whole chessboard to find the King
  findKing(COLOUR);

  CheckStatus checkStatus = CheckStatus::NotInCheck;

  //iterate through whole chessboard to see if any piece can capture the king
  for (int x_pos=0;x_pos<8;++x_pos) {
    for (int y_pos=0;y_pos<8;++y_pos) {
      if (board[x_pos][y_pos].isOccupied() && board[x_pos][y_pos].getChessPiece()->getColour()!=COLOUR) {
        if (isMoveLegal(x_pos, y_pos, kingIndex[0], kingIndex[1])) {
          if (checkStatus == CheckStatus::NotInCheck){
            checkStatus = CheckStatus::Check;
          }
          else {
            checkStatus = CheckStatus::DoubleCheck;
          }
          threat_x = x_pos;
          threat_y = y_pos;
        }
      }
    }
  }

  if (checkStatus == CheckStatus::Check) {
    if(!kingCanEscape(kingIndex, COLOUR) & !canBlockorCaptureThreat(threat_x, threat_y, COLOUR)) {
      std::cout << "Check Mate" << std::endl;
      return CheckStatus::CheckMate;
    }
  } else if (checkStatus == CheckStatus::DoubleCheck) {
    if (!kingCanEscape(kingIndex, COLOUR))
      return CheckStatus::CheckMate;
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

bool MoveEvaluator::movedIntoCheck(std::string COLOUR)
{
  findKing(COLOUR);
  return isVulnerableSquare(kingIndex[0], kingIndex[1]);
}

bool MoveEvaluator::canBlockorCaptureThreat(int threat_x, int threat_y, std::string COLOUR)
{
  for (int x_pos=0;x_pos<8;++x_pos) {
    for (int y_pos=0;y_pos<8;++y_pos) {
      if (board[x_pos][y_pos].isOccupied() && board[x_pos][y_pos].getChessPiece()->getColour()==COLOUR) {

        // step size is either 0, -1 or 1 walking from king to its threat
        int xstep = (threat_x - kingIndex[0] == 0 ? 0 : (threat_x - kingIndex[0])/abs(threat_x - kingIndex[0]));
        int ystep = (threat_y - kingIndex[1] == 0 ? 0 : (threat_y - kingIndex[1])/abs(threat_y - kingIndex[1]));

        int x = kingIndex[0];
        int y = kingIndex[1];

        while (x != threat_x || y != threat_y) {
          x += xstep;
          y += ystep;

            if (isMoveLegal(x_pos, y_pos, x, y)) {
              ChessPiecePtr threatRemover = board[x_pos][y_pos].getChessPiece();
              board[x][y].setChessPiece(threatRemover);
              board[x_pos][y_pos].setChessPiece(NULL);
              findKing(COLOUR);

              //was the threat remover pinned to the king before moving?
              if (!isVulnerableSquare(kingIndex[0], kingIndex[1])) {
                board[x][y].setChessPiece(NULL);
                board[x_pos][y_pos].setChessPiece(threatRemover);
                return true;
              }
              board[x][y].setChessPiece(NULL);
              board[x_pos][y_pos].setChessPiece(threatRemover);
            }
          }
        }
      }
    }
  return false;
}

bool MoveEvaluator::kingCanEscape(int* kingIndex, std::string COLOUR)
{
  for (int i=-1; i<2; ++i) {
    for (int j=-1; j<2; ++j) {

      int x = kingIndex[0]+i;
      int y = kingIndex[1]+j;

      if (isMoveLegal(kingIndex[0], kingIndex[1], x, y)) {
        ChessPiecePtr escapeSquare = NULL;
        if (board[x][y].isOccupied()) {
          escapeSquare = (board[x][y].getChessPiece());
        }
        board[x][y].setChessPiece(board[kingIndex[0]][kingIndex[1]]);
        board[kingIndex[0]][kingIndex[1]].setChessPiece(NULL);

        if (!isVulnerableSquare(kingIndex[0]+i, kingIndex[1]+j, COLOUR)) {
          board[kingIndex[0]][kingIndex[1]].setChessPiece(board[x][y]);
          board[kingIndex[0]+i][kingIndex[1]+j].setChessPiece(escapeSquare);
          return true;
        }

        board[kingIndex[0]][kingIndex[1]].setChessPiece(board[x][y]);
        board[kingIndex[0]+i][kingIndex[1]+j].setChessPiece(escapeSquare);

      }
    }
  }


  return false;
}

bool MoveEvaluator::isVulnerableSquare(int x,int y)
{
  for (int x_pos=0;x_pos<8;++x_pos) {
    for (int y_pos=0;y_pos<8;++y_pos) {
      if (board[x_pos][y_pos].isOccupied()
          && board[x_pos][y_pos].getChessPiece()->getColour()!=
            board[x][y].getChessPiece()->getColour()) {

        board[x_pos][y_pos].getChessPiece();
        if (isMoveLegal(x_pos, y_pos, x, y))
          return true;
      }
    }
  }
  return false;
}

bool MoveEvaluator::isVulnerableSquare(int x,int y,std::string COLOUR)
{
  for (int x_pos=0;x_pos<8;++x_pos) {
    for (int y_pos=0;y_pos<8;++y_pos) {
      if (board[x_pos][y_pos].isOccupied()
          && board[x_pos][y_pos].getChessPiece()->getColour()!=
            COLOUR) {

        board[x_pos][y_pos].getChessPiece();
        if (isMoveLegal(x_pos, y_pos, x, y))
          return true;
      }
    }
  }
  return false;
}

bool ChessBoard::isPathBlocked(int x_pos,int y_pos,int x_dest,int y_dest)
{
  std::string type = board[x_pos][y_pos].getChessPiece()->getType();
  int x_step = (x_dest - x_pos > 0 ? 1 : -1);
  int y_step = (y_dest - y_pos > 0 ? 1 : -1);
  //Diagonal motion
  if (abs(x_pos - x_dest) == abs(y_pos - y_dest)) {
    int distance = abs(y_dest - y_pos);
    while(distance>1) {
      //x_pos moves 1 step towards destination
      x_pos = x_pos + x_step;
      y_pos = y_pos + y_step;
      if (board[x_pos][y_pos].isOccupied()) {
        std::string debug1 = board[x_pos][y_pos].getChessPiece()->getType();
        return true;
      }
      distance--;
    }
    return false;
  }
  //Horizontal motion
  if (x_pos - x_dest == 0) {
    int distance = abs(y_dest - y_pos);
    while(distance>1) {
      y_pos = y_pos + y_step;
      if (board[x_pos][y_pos].isOccupied()) {
        return true;
      }
      distance--;
    }
    return false;
  }
  //vertical motion
  if (y_pos - y_dest == 0) {
    int distance = abs(x_dest - x_pos);
    while(distance>1) {
      x_pos = x_pos + x_step;
      if (board[x_pos][y_pos].isOccupied()) {
        return true;
      }
      distance--;
    }
    //coping with fact that pawn cannot capture vertically,hence the path is always blocked
    if (type=="PAWN" && board[x_dest][y_dest].isOccupied()) {
      return true;
    }
    return false;
  }
  return false;
}

bool MoveEvaluator::isMoveLegal(int x_pos,int y_pos,int x_dest,int y_dest)
{
  if (!((x_pos == x_dest) & (y_pos == y_dest))) {

    if ((x_dest>=0)&(x_dest<8)&(y_dest>=0)&(y_dest<8)) {

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
        if (board[x_pos][y_pos].getChessPiece()->getType() == "PAWN"){
          bool isCapturing = board[x_dest][y_dest].isOccupied();
          return (board[x_pos][y_pos].getChessPiece()->isValidMove(
                                                                   x_pos, y_pos,
                                                                   x_dest, y_dest,
                                                                   isCapturing));
        } else {
          return (board[x_pos][y_pos].getChessPiece()->isValidMove(
                                                                   x_pos,y_pos,
                                                                   x_dest,y_dest));
        }
      }
    }
  }
  return false;
}
