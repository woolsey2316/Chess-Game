#include "AnalysisScreen.h"

#include "GameScreen.h"
#include "global.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>

extern const double g_chessboardWidth;
extern const double g_chessboardHeight;
extern const double g_squareSize;
extern const int g_pixel_dx;
extern const int g_pixel_dy;
extern const double g_chessPieceScale_y;
extern const double g_chessPieceScale_x;

AnalysisScreen::AnalysisScreen()
{
  canReverseMoves = false;
  connector.connectToEngine((char *)"stockfish.exe");
  //computer ordinary match
  chessgame.setPlayer2Human(true);
  chessboard = chessgame.getChessBoard();
}

int AnalysisScreen::Run(sf::RenderWindow &App)
{
  loadChessSprites();

  bool running = true;
  while (running) {
  	mousePosition = sf::Mouse::getPosition(App);
  	sf::Vector2f mousePosF(static_cast<float>(mousePosition.x), static_cast<float>( mousePosition.y ));
  	sf::Event e;
  	while (App.pollEvent(e)) {
      switch (e.type) {
        case sf::Event::Closed: {
  	  	  App.close();
  	  	  break;
  	    }
  	    case sf::Event::MouseMoved: {

          if (backButton.getGlobalBounds().contains(mousePosF))
          {
            backButton.setColor(sf::Color(250, 20, 20));
          }
          else
          {
            backButton.setColor(sf::Color(250, 255, 255));
          }
          if (forwardButton.getGlobalBounds().contains(mousePosF))
          {
            forwardButton.setColor(sf::Color(255, 20, 20));
          }
          else
          {
            forwardButton.setColor(sf::Color(250, 255, 255));
          }
          break;
  	    }
  	    case sf::Event::MouseButtonPressed : {
          if (e.key.code == sf::Mouse::Left) {
            if (backButton.getGlobalBounds().contains(mousePosF)) {
              reverseMove(App);
              buttonPressed = true;
            }
            dragChessPiece();

          }
          break;
  	    }
  	    case (sf::Event::MouseButtonReleased): {
          if (e.key.code == sf::Mouse::Left) {
            if (touchedByPlayer > -1 && !buttonPressed) {
              moveSprite(letGoOfPiece());
            }
            buttonPressed = false;
          }
        }
          break;
      }
      if (isPressed) {
        chesspieceSprite[touchedByPlayer].setPosition(mousePosition.x-dx,
                                                      mousePosition.y-dy);
      }
      refreshDisplay(App);

      if (chessgame.getTurn() == "BLACK" && !chessgame.isHumanOpponent()
          && moveType != MoveType::CheckMate) {
        computerMove();
      }
    }
  }
  return -1;
}

void AnalysisScreen::refreshDisplay(sf::RenderWindow &App) {
  App.clear();
  App.draw(sidePanel);
  App.draw(boardSprite);
  for (int i = 0; i < 32; ++i) {
      App.draw(chesspieceSprite[i]);
  }
  App.draw(backButton);
  App.draw(forwardButton);
  App.display();
}
void AnalysisScreen::computerMove()
{
  std::string compMove = connector.getCompMove(
                  chessgame.getEntireMoveHistory(),
                  comp);
  std::vector<std::array<std::string, 2>> variations = connector.getScore(
                  chessgame.getEntireMoveHistory(),
                  comp);
  for (const auto & elem : variations) {
     std::cout << elem[0] << " | " <<  elem[1] << std::endl;
  }

  if (compMove == "error") {
    std::cout << "error" << std::endl;
    return;
  }
  oldCoordinates.setCoords(compMove.substr(0,2));
  newCoordinates.setCoords(compMove.substr(2,2));
  moveType = chessboard.playerMove(chessgame.getTurn(),
                                   oldCoordinates.getIntegerCoords()[1],
                                   oldCoordinates.getIntegerCoords()[0],
                                   newCoordinates.getIntegerCoords()[1],
                                   newCoordinates.getIntegerCoords()[0]);
  for (int i=0;i<32;i++){
    if (chesspieceSprite[i].getGlobalBounds()
        .contains(oldCoordinates.getPixelCoords())) {
      touchedByComputer=i;
    }
  }
  move(oldCoordinates, newCoordinates, moveType, touchedByComputer);
  chessgame.updateEntireMoveHistory(compMove + " ");
  chessgame.nextTurn();
}

void AnalysisScreen::dragChessPiece()
{
  for(int i = 0; i < 32; ++i){
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

MoveType AnalysisScreen::letGoOfPiece()
{
    isPressed = false;
    sf::Vector2f p = chesspieceSprite[touchedByPlayer].getPosition() +
        sf::Vector2f(0.5*g_squareSize, 0.5*g_squareSize);
    newCoordinates.setCoords(p);
    newPos = newCoordinates.getPixelCoords();

    int* dest = newCoordinates.getIntegerCoords();
    int* start = oldCoordinates.getIntegerCoords();

    return chessboard.playerMove(chessgame.getTurn(),
                                 start[1], start[0], dest[1], dest[0]);

}

void AnalysisScreen::moveSprite(MoveType moveType)
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

void AnalysisScreen::loadChessSprites()
{
  sidePanel_t.loadFromFile("images/wood-background.jpg");
  sidePanel.setTexture(sidePanel_t);

  gameBoard_t.loadFromFile("images/chessboard4.png");
  boardSprite.setTexture(gameBoard_t);
  boardSprite.scale(g_chessboardWidth/gameBoard_t.getSize().x,
      g_chessboardWidth/gameBoard_t.getSize().x);
  boardSprite.setPosition(g_pixel_dx, g_pixel_dy);

  backButton_t.loadFromFile("images/backButton.png");
  backButton.setTexture(backButton_t);
  backButton.setPosition(190 + g_pixel_dx, 555 + g_pixel_dy);
  forwardButton_t.loadFromFile("images/forwardButton.png");
  forwardButton.setTexture(forwardButton_t);
  forwardButton.setPosition(295 + g_pixel_dx, 555 + g_pixel_dy);

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

void AnalysisScreen::move(Coordinates oldCoords, Coordinates newCoords,
                   MoveType moveType, int touchedByPlayer)
{
  sf::Vector2f start = oldCoords.getPixelCoords();
  sf::Vector2f dest = newCoords.getPixelCoords();

  if (moveType == MoveType::Capture || moveType == MoveType::EmptySquare) {

    for(int i=0;i<32;i++) {
      if (chesspieceSprite[i].getPosition() == dest) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }

    chesspieceSprite[touchedByPlayer].setPosition(dest);

  } else if (moveType == MoveType::Promotion) {

    for(int i=0;i<32;i++) {
      if (chesspieceSprite[i].getPosition() == dest) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }

    promotePawn(touchedByPlayer, dest);

  } else if (moveType == MoveType::enPassant) {

    for(int i=0;i<32;i++) {
      if ((chesspieceSprite[i].getPosition().x == dest.x)
          & (chesspieceSprite[i].getPosition().y == start.y)) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }

    chesspieceSprite[touchedByPlayer].setPosition(dest);

  } else if (moveType == MoveType::Castle) {

    for(int i=0;i<32;i++) {
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

    for(int i=0;i<32;i++) {
      if (chesspieceSprite[i].getPosition() == dest) {
        chesspieceSprite[i].setPosition(-100,-100);
      }
    }
    chesspieceSprite[touchedByPlayer].setPosition(dest);
    chessgame.saveGame();
  }
}

void AnalysisScreen::promotePawn(int index, sf::Vector2f dest)
{
  texture[index].loadFromFile("images/"
      + boost::algorithm::to_lower_copy(chessboard.getPromotingColour())
      + "_"
      + boost::algorithm::to_lower_copy(chessboard.getPromotingPiece())
      + ".png");
  chesspieceSprite[index].setTexture(texture[index]);
  chesspieceSprite[index].setPosition(dest);
}

void AnalysisScreen::reverseMove(sf::RenderWindow &App)
{
  if (!isPressed) {
    chessgame.reverseMove();
    chessboard.reset();
    chessgame.setTurnToWhite();
    loadChessSprites();

    std::string moves = chessgame.getEntireMoveHistory();
    for(int i=0;i<moves.length();i+=5) {
      std::string coords = moves.substr(i,4);
      oldCoordinates.setCoords(coords.substr(0,2));
      newCoordinates.setCoords(coords.substr(2,2));
      moveType = chessboard.playerMove(chessgame.getTurn(),
                                     oldCoordinates.getIntegerCoords()[1],
                                     oldCoordinates.getIntegerCoords()[0],
                                     newCoordinates.getIntegerCoords()[1],
                                     newCoordinates.getIntegerCoords()[0]);
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
