#include "ChessGame.h"
#include "ChessBoard.h"

#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <iomanip>


#include <nlohmann/json.hpp>

using Json = nlohmann::json;

ChessGame::ChessGame()
{
  playerTurn = "WHITE";
  player2Human = false;
  ChessBoard chessBoard;
}

ChessGame::ChessGame(bool player2human)
{
  playerTurn = "WHITE";
  player2Human = player2human;
  ChessBoard chessBoard;
}

void ChessGame::nextTurn()
{
  playerTurn = (playerTurn  == "WHITE" ? "BLACK" : "WHITE");
}

void ChessGame::setTurnToWhite()
{
  playerTurn = "WHITE";
}

void ChessGame::loadSavedGame(std::string fileName) {
  setTurnToWhite();
  hasEnded = false;
  std::ifstream infile;
  entireMoveHistory = "";
  moveList.clear();
  infile.open(fileName + ".txt");
  if (infile.fail()) {
    std::cerr << "Error Openning File";
    exit(1);
  }
  std::string move;
  while (!infile.eof()) {
    infile >> move;
    updateEntireMoveHistory(move + " ");
  }
  infile.close();
}

std::string ChessGame::getTurn()
{
  return playerTurn ;
}

ChessBoard ChessGame::getChessBoard()
{
  return chessBoard ;
}

bool ChessGame::isHumanOpponent()
{
  return player2Human;
}

void ChessGame::updateEntireMoveHistory(std::string move)
{
  moveList.push_back(move);
  entireMoveHistory += move;
}

std::string ChessGame::getEntireMoveHistory()
{
  return entireMoveHistory;
}

void ChessGame::setPlayer2Human(bool p2h)
{
  player2Human = p2h;
}

void ChessGame::reverseMove()
{
  if (!moveList.empty()) {
    moveList.pop_back();
    if (entireMoveHistory.length() > 12) {
      entireMoveHistory.erase(entireMoveHistory.length()-5,5);
    } else {
      entireMoveHistory = "";
    }
    std::cout << std::endl << (entireMoveHistory);
  }
}

Computer& ChessGame::getComputer()
{
  return computer;
}

void ChessGame::saveGame()
{
  std::ofstream outf("data/save.txt");
  if (!outf)
  {
    std::cerr << "could not be opened for writing!" << std::endl;
    exit(1);
  }
  for (const auto & elem : moveList) {
    outf << elem << std::endl;
  }
  outf.close();

  std::ofstream outfile("data/user.txt");
  if (!outfile)
  {
    std::cerr << "could not be opened for writing" << std::endl;
    exit(1);
  }
  outfile << std::setw(4) << player.to_json() << std::endl;
  outfile.close();
}

void ChessGame::finishGame(MoveType result)
{
  if (!hasEnded) {
    player.updateScore(computer.getEloRating(), result, playerTurn);
    saveGame();
    hasEnded = true;
  }

}

bool ChessGame::hasFinished()
{
  return hasEnded;
}

void ChessGame::setMoveNumber(int num) {
  moveNumber = num;
}

void ChessGame::reset() {
  entireMoveHistory = "";
  chessBoard.reset();
  setMoveNumber(0);
  setTurnToWhite();
}
