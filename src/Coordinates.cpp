#include "Coordinates.h"

extern const int g_chessboardWidth;
extern const int g_chessboardHeight;
extern const double g_squareSize;
extern const int g_pixel_dx;
extern const int g_pixel_dy;

Coordinates::Coordinates()
{
  integerCoords[0] = -1;
  integerCoords[1] = -1;
}

Coordinates::Coordinates(std::string coords)
{
  setCoords(coords);
}

sf::Vector2f Coordinates::getPixelCoords()
{
  return sf::Vector2f(integerCoords[0]*g_squareSize + g_pixel_dx + 0.0625*g_squareSize,
                      integerCoords[1]*g_squareSize + g_pixel_dy + 0.125*g_squareSize);
}

sf::Vector2f Coordinates::RankFiletoPixelCoords(char a,char b)
{
   int x = int(a) - 97;
   int y = 7 - int(b) + 49;
   return sf::Vector2f(x*g_squareSize + g_pixel_dx,y*g_squareSize + g_pixel_dy);
}

void Coordinates::setIntegerCoords(int x_pos, int y_pos)
{
  integerCoords[0] = x_pos;
  integerCoords[1] = y_pos;
}

int *Coordinates::getIntegerCoords()
{
  return integerCoords;
}

void Coordinates::setCoords(sf::Vector2f pixelCoords)
{
  integerCoords[0] = (pixelCoords.x - g_pixel_dx) / g_squareSize;
  integerCoords[1] = (pixelCoords.y - g_pixel_dy) / g_squareSize;
}

void Coordinates::setCoords(std::string coords)
{
  integerCoords[0] = int(coords[0]) - 97;
  integerCoords[1] = 7 - int(coords[1]) + 49;
}

std::string Coordinates::getRankFileIndex()
{
  std::string s = "";
  s += char(integerCoords[0] + 97);
  s += char(7 - integerCoords[1] + 49);
  return s;
}
