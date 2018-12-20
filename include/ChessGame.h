#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "Player.h"
#include "ChessBoard.h"
#include "Computer.h"
#include "MoveType.h"

#include <fstream>
#include <string>

class ChessGame {
 private:
  std::string playerTurn;
  std::string entireMoveHistory;
  std::vector<std::string> moveList;
  int moveNumber;
  bool savedGame;
	bool player2Human;
	bool hasEnded;
	ChessBoard chessBoard;
	Computer computer;
	Player player;

 public:
  ChessGame();
  void finishGame(MoveType result);
  void saveGame();
  bool hasFinished();
  ChessGame(bool player2human, std::string difficulty_);
  ChessGame(bool player2human);
  ChessBoard getChessBoard();
  void loadSavedGame(std::string fileName);
  std::string getDifficulty();
  int getMoveNumber();
  void setMoveNumber(int num);
	void nextTurn();
	void setTurnToWhite();
	bool isHumanOpponent();
	std::string getTurn();
	std::string getEntireMoveHistory();
	void updateEntireMoveHistory(std::string);
	void setPlayer2Human(bool p2h);
	Computer & getComputer();
	void reverseMove();
	void reset();
};

#endif

