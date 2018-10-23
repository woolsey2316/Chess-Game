#include "Display.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <chrono>
#include <thread>

extern const double g_chessboardWidth;
extern const double g_chessboardHeight;
extern const double g_squareSize;
extern const int g_pixel_dx;
extern const int g_pixel_dy;
extern const double g_chessPieceScale_y;
extern const double g_chessPieceScale_x;

Display::Display()
{
  connector.connectToEngine((char *)"stockfish.exe");
  //historical match
  chessgame.loadSavedGame("match");
  //computer ordinary match
  chessgame.setPlayer2Human(true);
  chessgame.setDifficulty(5);
  chessboard = chessgame.getChessBoard();

  while (window.isOpen()) {
  	mousePosition = sf::Mouse::getPosition(window);
  	sf::Event e;
  	while (window.pollEvent(e)) {
      switch (e.type) {
        case sf::Event::Closed: {
  	  	  window.close();
  	  	  break;
  	    }
  	    case sf::Event::MouseMoved: {
          sf::Vector2i mousePos = sf::Mouse::getPosition(window);
          sf::Vector2f mousePosF( static_cast<float>(mousePos.x), static_cast<float>( mousePos.y ) );
          if (backButton.getGlobalBounds().contains(mousePosF))
          {
            backButton.setColor(sf::Color(250, 20, 20));
          }
          else
          {
            backButton.setColor(sf::Color(255, 255, 255));
          }
  	    }
  	    case (sf::Event::MouseButtonPressed): {
          if (e.key.code == sf::Mouse::Left) {
            dragChessPiece();
            if (isStillMoving) {
              chesspieceSprite[touchedByPlayer].setPosition(mousePosition.x-dx,
                                                            mousePosition.y-dy);
            }
          }
  	    }
  	    case (sf::Event::MouseButtonReleased): {
          if (e.key.code == sf::Mouse::Left) {
            updateDisplay(letGoOfPiece());
            }
          }
      }
    if (isStillMoving) {
      chesspieceSprite[touchedByPlayer].setPosition(mousePosition.x-dx,
                                                    mousePosition.y-dy);
    }
    window.clear();
    window.draw(boardSprite);
    for (int i = 0; i < 32; ++i) {
        window.draw(chesspieceSprite[i]);
    }
    window.display();

    if (chessgame.getTurn() == "BLACK" && !chessgame.isHumanOpponent() && moveType != MoveType::CheckMate) {
      computerMove();
    }
  }
}
}

void Display::computerMove()
{
  std::string compMove = connector.getCompMove(
                  chessgame.getEntireMoveHistory(),
                  chessgame.getDifficulty());
  std::cout << compMove << std::endl;
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

void Display::dragChessPiece()
{
  for(int i = 0; i < 32; ++i){
    if (chesspieceSprite[i].getGlobalBounds().contains(mousePosition.x,
                                                       mousePosition.y)) {
      isStillMoving = true; touchedByPlayer = i;
      dx = mousePosition.x - chesspieceSprite[i].getPosition().x;
      dy = mousePosition.y - chesspieceSprite[i].getPosition().y;
      oldPos = chesspieceSprite[i].getPosition();
      oldCoordinates.setCoords(oldPos);
    }
  }
}

MoveType Display::letGoOfPiece()
{
  isStillMoving = false;
  sf::Vector2f p = chesspieceSprite[touchedByPlayer].getPosition() +
      sf::Vector2f(g_squareSize/2,g_squareSize/2);
  newCoordinates.setCoords(p);
  newPos = newCoordinates.getPixelCoords();

  int* dest = newCoordinates.getIntegerCoords();
  int* start = oldCoordinates.getIntegerCoords();

  return chessboard.playerMove(chessgame.getTurn(),
                               start[1], start[0], dest[1], dest[0]);
}

void Display::updateDisplay(MoveType moveType)
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

void Display::loadChessSprites()
{

  gameBoard.loadFromFile("images/chessboard.png");

  boardSprite.setTexture(gameBoard);
  boardSprite.scale(1.0*g_chessboardWidth/gameBoard.getSize().x,
                    1.0*g_chessboardHeight/gameBoard.getSize().y);

  int index=0;
  for(int i=0;i<8;++i) {
    for (int j=0;j<8;++j) {
      if (board_[i][j] == "empty") {
        continue;
      }
      texture[index].loadFromFile("images/" + board_[i][j] + ".png");
      texture[index].setSmooth(true);

      chesspieceSprite[index].setTexture(texture[index]);
      chesspieceSprite[index].scale(
          g_chessPieceScale_y*g_squareSize/texture[index].getSize().x,
          g_chessPieceScale_x*g_squareSize/texture[index].getSize().y);
      chesspieceSprite[index].setPosition(g_squareSize*j + g_pixel_dx,
                                          g_squareSize*i + g_pixel_dy);
      index++;
    }
  }
}

void Display::move(Coordinates oldCoords, Coordinates newCoords,
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

  }
}

void Display::promotePawn(int index, sf::Vector2f dest)
{
  texture[index].loadFromFile("images/"
      + boost::algorithm::to_lower_copy(chessboard.getPromotingColour())
      + "_"
      + boost::algorithm::to_lower_copy(chessboard.getPromotingPiece())
      + ".png");
  texture[index].setSmooth(true);
  chesspieceSprite[index].setTexture(texture[index]);
  chesspieceSprite[index].setPosition(dest);
}

void Display::loadMainWindow()
{
  while (window.isOpen()) {
  	mousePosition = sf::Mouse::getPosition(window);
  	sf::Event e;
  	while (window.pollEvent(e)) {
      switch (e.type) {
        case sf::Event::Closed: {
  	  	  window.close();
  	  	  break;
  	    }
  	    case sf::Event::MouseMoved: {
          sf::Vector2i mousePos = sf::Mouse::getPosition(window);
          sf::Vector2f mousePosF( static_cast<float>(mousePos.x), static_cast<float>( mousePos.y ) );
          if (backButton.getGlobalBounds().contains(mousePosF))
          {
            backButton.setColor(sf::Color(250, 20, 20));
          }
          else
          {
            backButton.setColor(sf::Color(255, 255, 255));
          }
  	    }
  	    case (sf::Event::MouseButtonPressed): {
          if (e.key.code == sf::Mouse::Left) {
            dragChessPiece();
            if (isStillMoving) {
              chesspieceSprite[touchedByPlayer].setPosition(mousePosition.x-dx,
                                                            mousePosition.y-dy);
            }
          }
  	    }
  	    case (sf::Event::MouseButtonReleased): {
          if (e.key.code == sf::Mouse::Left) {
            updateDisplay(letGoOfPiece());
            }
          }
      }
    if (isStillMoving) {
      chesspieceSprite[touchedByPlayer].setPosition(mousePosition.x-dx,
                                                    mousePosition.y-dy);
    }
    window.clear();
    window.draw(boardSprite);
    for (int i = 0; i < 32; ++i) {
        window.draw(chesspieceSprite[i]);
    }
    window.display();

    if (chessgame.getTurn() == "BLACK" && !chessgame.isHumanOpponent() && moveType != MoveType::CheckMate) {
      computerMove();
    }
  }
}
}

void Display::reverseMove()
{
  if (chessgame.getEntireMoveHistory().length() > 6) {
    chessgame.reverseMove();
    chessgame.nextTurn();
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
    }
  }
}
