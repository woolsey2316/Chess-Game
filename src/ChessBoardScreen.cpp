#include "ChessBoardScreen.h"

#include "CheckMateScreen.h"
#include "global.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>
#include <map>
#include <map>
#include <map>

extern const double g_chessboardWidth;
extern const double g_chessboardHeight;
extern const double g_squareSize;
extern const float g_pixel_dx;
extern const float g_pixel_dy;
extern const double g_chessPieceScale_y;
extern const double g_chessPieceScale_x;

int ChessBoardScreen::Run(sf::RenderWindow &App)
{
  return 1;
}

void ChessBoardScreen::computerMove()
{
  std::string compMove = connector.getCompMove(
                  chessgame.getEntireMoveHistory(),
                  comp);
  char promotionType = compMove[4];
  if (compMove == "error") {
    std::cout << "error" << std::endl;
    return;
  }
  std::cout << compMove.substr(0,4) << std::endl;
  oldCoordinates.setCoords(compMove.substr(0,2));
  newCoordinates.setCoords(compMove.substr(2,2));
  moveType = chessboard.playerMove(chessgame.getTurn(),
                                   oldCoordinates.getIntegerCoords()[1],
                                   oldCoordinates.getIntegerCoords()[0],
                                   newCoordinates.getIntegerCoords()[1],
                                   newCoordinates.getIntegerCoords()[0],
                                   promotionType);
  for (size_t i=0;i<chesspieceSprite.size();i++){
    if (chesspieceSprite[i].getGlobalBounds()
        .contains(oldCoordinates.getPixelCoords())) {
      touchedByComputer=i;
    }
  }
  move(oldCoordinates, newCoordinates, moveType, touchedByComputer);
  chessgame.updateEntireMoveHistory(compMove.substr(0,4) + " ");
  chessgame.nextTurn();
}

void ChessBoardScreen::dragChessPiece()
{
  for(size_t i = 0; i < chesspieceSprite.size(); ++i){
    if (chesspieceSprite[i].getGlobalBounds().contains(mousePosition.x,
                                                       mousePosition.y)) {
      isPressed = true; touchedByPlayer = i;
      dx = mousePosition.x - chesspieceSprite[i].getPosition().x;
      dy = mousePosition.y - chesspieceSprite[i].getPosition().y;
      oldPos = chesspieceSprite[i].getPosition();
      oldCoordinates.setCoords(oldPos);
    }
  }

  if (isPressed) {
    chesspieceSprite[touchedByPlayer].setPosition(mousePosition.x-dx,
                                                  mousePosition.y-dy);
  }
}

void ChessBoardScreen::getPositionScores() {
  std::map<float,std::string, std::greater<float>> scores = connector.getScore(chessgame.getEntireMoveHistory(), comp);
  variation.clear();
  int i = 0;
    std::map<float,std::string, std::greater<float>>::iterator it = scores.begin();
    while (it != scores.end()) {
      sf::Text moveVariation;
      moveVariation.setFont(Font);
      moveVariation.setString(std::to_string((*it).first).substr(0,6) + " : " + (*it).second);
      moveVariation.setCharacterSize(11);
      moveVariation.setPosition({605.f, 160.f + i*45});
      moveVariation.setFillColor(sf::Color(105,105,105));
      moveVariation.getLocalBounds();

      variation.push_back(moveVariation);
      it++;
      i++;
      if (i == 8) {
        break;
      }
    }
}


MoveType ChessBoardScreen::letGoOfPiece()
{
    isPressed = false;
    sf::Vector2f p = chesspieceSprite[touchedByPlayer].getPosition() +
        sf::Vector2f(0.5*g_squareSize, 0.5*g_squareSize);
    newCoordinates.setCoords(p);
    newPos = newCoordinates.getPixelCoords();

    int* dest = newCoordinates.getIntegerCoords();
    int* start = oldCoordinates.getIntegerCoords();
    return chessboard.playerMove(chessgame.getTurn(),
                                 start[1], start[0], dest[1], dest[0], ' ');


}

void ChessBoardScreen::moveSprite(MoveType moveType)
{
  if (moveType == MoveType::Invalid) {
    chesspieceSprite[touchedByPlayer].setPosition(sf::Vector2f(oldPos.x,oldPos.y));
  } else {
    chessgame.updateEntireMoveHistory(oldCoordinates.getRankFileIndex()
                                      + newCoordinates.getRankFileIndex()
                                      + " ");
    std::cout << oldCoordinates.getRankFileIndex()
        + newCoordinates.getRankFileIndex() + " " << std::endl;
    move(oldCoordinates, newCoordinates, moveType, touchedByPlayer);
    chessgame.nextTurn();
    oldPos = newPos;
  }
}

void ChessBoardScreen::loadChessSprites()
{
  gameBoard_t.loadFromFile(chessBoardFile);
  boardSprite.setTexture(gameBoard_t);
  boardSprite.setPosition(g_pixel_dx, g_pixel_dy);

  background_t.loadFromFile("images/wood-background.jpg");
  background.setTexture(background_t);

  mainMenu_t.loadFromFile("images/main_menu.png");
  mainMenu.setTexture(mainMenu_t);
  mainMenu.setPosition({860, g_pixel_dy + 10});

  retry_t.loadFromFile("images/retry.png");
  retry.setTexture(retry_t);
  retry.setPosition({800, g_pixel_dy + 12});

  for (size_t i = 0; i < rectangle.size(); ++i) {
    rectangle[i].setFillColor(sf::Color(200,200,200));
    rectangle[i].setSize(sf::Vector2f(40, 40));
    rectangle[i].setPosition(795+i*60, g_pixel_dy + 5);
  }

  int index=0;
  for(int i=0;i<8;++i) {
    for (int j=0;j<8;++j) {
      if (board_[i][j] == "empty") {
        continue;
      }
      texture[index].loadFromFile("images/" + board_[i][j] + ".png");

      chesspieceSprite[index].setTexture(texture[index]);
      chesspieceSprite[index].setPosition(g_squareSize*j + 0.0625*g_squareSize + g_pixel_dx,
                                          g_squareSize*i + 0.125*g_squareSize + g_pixel_dy);
      index++;
    }
  }
}

void ChessBoardScreen::move(Coordinates oldCoords, Coordinates newCoords,
                   MoveType moveType, int touchedByPlayer)
{
  sf::Vector2f start = oldCoords.getPixelCoords();
  sf::Vector2f dest = newCoords.getPixelCoords();

  if (moveType == MoveType::Capture || moveType == MoveType::EmptySquare) {

    for(size_t i=0;i<chesspieceSprite.size();i++) {
      if (chesspieceSprite[i].getPosition() == dest) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }

    chesspieceSprite[touchedByPlayer].setPosition(dest);

  } else if (moveType == MoveType::Promotion) {

    for(size_t i=0;i<chesspieceSprite.size();i++) {
      if (chesspieceSprite[i].getPosition() == dest) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }

    promotePawn(touchedByPlayer, dest);

  } else if (moveType == MoveType::enPassant) {

    for(size_t i=0;i<chesspieceSprite.size();i++) {
      if ((chesspieceSprite[i].getPosition().x == dest.x)
          & (chesspieceSprite[i].getPosition().y == start.y)) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }

    chesspieceSprite[touchedByPlayer].setPosition(dest);

  } else if (moveType == MoveType::Castle) {

    for(size_t i=0;i<chesspieceSprite.size();i++) {
      if (chesspieceSprite[i].getPosition() == start) {
        touchedByPlayer = i;
      }
    }
    chesspieceSprite[touchedByPlayer].setPosition(dest);

    std::string playerMove = oldCoordinates.getRankFileIndex()
        + newCoordinates.getRankFileIndex();

    if (playerMove=="e1g1") {
      oldCoordinates.setCoords("h1");
      newCoordinates.setCoords("f1");
      move(oldCoordinates, newCoordinates, moveType, touchedByPlayer);
    } else if (playerMove=="e8g8") {
      oldCoordinates.setCoords("h8");
      newCoordinates.setCoords("f8");
      move(oldCoordinates, newCoordinates, moveType, touchedByPlayer);
    } else if (playerMove=="e1c1") {
      oldCoordinates.setCoords("a1");
      newCoordinates.setCoords("d1");
      move(oldCoordinates, newCoordinates, moveType, touchedByPlayer);
    } else if (playerMove=="e8c8") {
      oldCoordinates.setCoords("a8");
      newCoordinates.setCoords("d8");
      move(oldCoordinates, newCoordinates, moveType, touchedByPlayer);
    }

  } else if (moveType == MoveType::CheckMate) {

    for(size_t i=0;i<chesspieceSprite.size();i++) {
      if (chesspieceSprite[i].getPosition() == dest) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }
    CheckMateScreen checkMateScreen;
    chesspieceSprite[touchedByPlayer].setPosition(dest);
    chessgame.finishGame(moveType);

  } else if (moveType == MoveType::StaleMate || moveType == MoveType::Draw) {
    for(size_t i=0;i<chesspieceSprite.size();i++) {
      if (chesspieceSprite[i].getPosition() == dest) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }
    chesspieceSprite[touchedByPlayer].setPosition(dest);
    chessgame.finishGame(moveType);
    std::cout << "Draw" << std::endl;

  } else if (moveType == MoveType::Invalid) {
    std::cout << "invalid Move" << std::endl;
  }

}

void ChessBoardScreen::promotePawn(int index, sf::Vector2f dest)
{
  texture[index].loadFromFile("images/"
      + boost::algorithm::to_lower_copy(chessboard.getPromotingColour())
      + "_"
      + boost::algorithm::to_lower_copy(chessboard.getPromotingPiece())
      + ".png");
  chesspieceSprite[index].setTexture(texture[index]);
  chesspieceSprite[index].setPosition(dest);
}

void ChessBoardScreen::loadPosition(sf::RenderWindow &App)
{
  if (!isPressed) {
    chessboard.reset();
    loadChessSprites();

    std::string moves = chessgame.getEntireMoveHistory();
    for(size_t i=0;i<moves.length();i+=5) {

      std::string coords = moves.substr(i,4);
      std::cout << coords << std::endl;
      oldCoordinates.setCoords(coords.substr(0,2));
      newCoordinates.setCoords(coords.substr(2,2));
      moveType = chessboard.playerMove(chessgame.getTurn(),
                                     oldCoordinates.getIntegerCoords()[1],
                                     oldCoordinates.getIntegerCoords()[0],
                                     newCoordinates.getIntegerCoords()[1],
                                     newCoordinates.getIntegerCoords()[0],
                                       ' ');
      for (size_t i=0;i<chesspieceSprite.size();i++){
        if (chesspieceSprite[i].getGlobalBounds()
            .contains(oldCoordinates.getPixelCoords())) {
          touchedByComputer=i;
        }
      }
      move(oldCoordinates, newCoordinates, moveType, touchedByComputer);
      chessgame.nextTurn();
    }
    refreshDisplay(App);
  }
}

void ChessBoardScreen::reverseMove(sf::RenderWindow &App)
{
  if (!isPressed) {
    chessgame.reverseMove();
    chessboard.reset();
    chessgame.setTurnToWhite();
    loadChessSprites();

    std::string moves = chessgame.getEntireMoveHistory();
    for(size_t i=0;i<moves.length();i+=5) {
      std::string coords = moves.substr(i,4);
      oldCoordinates.setCoords(coords.substr(0,2));
      newCoordinates.setCoords(coords.substr(2,2));
      moveType = chessboard.playerMove(chessgame.getTurn(),
                                     oldCoordinates.getIntegerCoords()[1],
                                     oldCoordinates.getIntegerCoords()[0],
                                     newCoordinates.getIntegerCoords()[1],
                                     newCoordinates.getIntegerCoords()[0],
                                       ' ');
      for (int i=0;i<32;i++){
        if (chesspieceSprite[i].getGlobalBounds()
            .contains(oldCoordinates.getPixelCoords())) {
          touchedByComputer=i;
        }
      }
      move(oldCoordinates, newCoordinates, moveType, touchedByComputer);
      chessgame.nextTurn();
      refreshDisplay(App);
    }
  }
}

void ChessBoardScreen::resetGame() {
  touchedByPlayer = -1;
  chessboard.reset();
  chessgame.setTurnToWhite();
  loadChessSprites();
}

void ChessBoardScreen::setChessBoardFile(std::string file)
{
  chessBoardFile = file;
  gameBoard_t.loadFromFile(chessBoardFile);
  boardSprite.setTexture(gameBoard_t);
  boardSprite.setPosition(g_pixel_dx, g_pixel_dy);
}
