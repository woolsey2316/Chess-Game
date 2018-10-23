#ifndef COORDINATES_H
#define COORDINATES_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Coordinates {
 public:
  Coordinates();
  Coordinates(std::string coords);
  Coordinates(int pos[]);
  void setCoords(std::string pos);
  void setCoords(int pos[]);
  void setCoords(sf::Vector2f pos);
  sf::Vector2f chessSquareIndex(int x, int y);
  sf::Vector2f RankFiletoPixelCoords(char a, char b);
  std::string toRankFileIndex(sf::Vector2f p);
  int *RankFiletoInteger(std::string chessMove);
  toIntegerCoords(int x_pos, int y_pos);

  std::string getRankFileIndex();
  int *getIntegerCoords();
  sf::Vector2f getPixelCoords();
  int getIntegerCoords(std::string coords);
  void setPixelCoords();
  void setRankFileIndex();
  void setIntegerCoords(int x_pos, int y_pos);

 private:
    int integerCoords[2];
};

#endif // COORDINATES_H
