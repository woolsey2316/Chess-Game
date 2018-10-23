#ifndef COMPUTER_H
#define COMPUTER_H

#include <string>

class Computer
{
  public:
    Computer();
    Computer(std::string colour);
    std::string getMoveTime();
    std::string getSkillLevel();
    std::string getDepth();
    void setDifficulty(int difficulty);
    int getEloRating();
    int getDifficulty();
  protected:

  private:
    std::string COLOUR;
    int EloRating[8] = {800,1000,1400,1600,1700,1900,2000,2200};
    std::string skillLevel[8] = {"3","6","9","11","14","17","20","20"};
    std::string moveTime[8] = {"50","100","150","200","250","300","350","400"};
    std::string depth[8] = {"1","2","3","4","6","8","10","12"};
    int difficulty;
};

#endif // COMPUTER_H
