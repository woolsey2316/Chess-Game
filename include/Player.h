#ifndef PLAYER_H
#define PLAYER_H

#include "MoveType.h"

#include <nlohmann/json.hpp>

#include <string>

using Json = nlohmann::json;

class Player
{
 public:
    Player();
    Player(std::string colour);
    void saveUserStatistics();
    Player(std::string colour, std::string userRecord);
    void updateScore(int opponentRating, MoveType result, std::string turn);
    void loadData();
    Json to_json() const;


protected:

private:
  std::string COLOUR = "WHITE";
  int numberOfWins;
  int numberOfDefeats;
  int numberOfDraws;
  int winLossRatio;
  int numberOfGamesPlayed;
  int rating;
};

#endif // PLAYER_H
