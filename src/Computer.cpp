#include "Computer.h"
#include <string>
#include <vector>

Computer::Computer(std::string colour){
  COLOUR = colour;
  difficulty = 5;
}

Computer::Computer(){
  COLOUR = "BLACK";
  difficulty = 5;
}

void Computer::setDifficulty(int diff)
{
  difficulty = diff;
}

std::string Computer::getDepth()
{
  return depth[difficulty];
}

std::string Computer::getSkillLevel()
{
  return skillLevel[difficulty];
}

int Computer::getDifficulty()
{
  return difficulty;
}

std::string Computer::getMoveTime()
{
  return moveTime[difficulty];
}

int Computer::getEloRating()
{
  return EloRating[difficulty];
}
