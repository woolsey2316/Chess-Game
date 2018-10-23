#include "Player.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using Json = nlohmann::json;

Json Player::to_json() const {
  return Json{{"COLOUR", COLOUR}, {"numberOfWins", numberOfWins},
  {"numberOfDefeats", numberOfDefeats}, {"numberOfDraws", numberOfDraws},
  {"rating", rating}};
}

Player::Player()
{
  COLOUR = "WHITE";
  numberOfWins = 0;
  numberOfDefeats = 0;
  numberOfGamesPlayed = 0;
  numberOfDraws = 0;
  rating = 800;
}

Player::Player(std::string colour)
{
  COLOUR = colour;
  numberOfWins = 0;
  numberOfDefeats = 0;
  numberOfGamesPlayed = 0;
  numberOfDraws = 0;
  rating = 800;
}

void Player::updateScore(int opponentRating, MoveType result, std::string turn)
{
  if (result == MoveType::StaleMate || result == MoveType::Draw) {
    rating += (opponentRating - rating)/20 + 1;
  } else if (COLOUR == turn) {
    rating += (opponentRating - rating)/20 + 5;
  } else {
    rating += (opponentRating - rating)/40 - 5;
  }
}

void Player::loadData()
{
  std::ifstream infile("userData.json");
  Json userdata_json;
  infile >> userdata_json;

  COLOUR = userdata_json["COLOUR"];
  numberOfWins = userdata_json["numberOfWins"];
  numberOfDefeats = userdata_json["numberOfDefeats"];
  numberOfGamesPlayed = userdata_json["numberOfGamesPlayed"];
  numberOfDraws = userdata_json["numberOfDraws"];
  rating = userdata_json["rating"];

}
