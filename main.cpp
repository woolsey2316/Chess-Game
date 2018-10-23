#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <time.h>
#include <string>

#include "Connector.h"
#include "Coordinates.h"
#include "ChessGame.h"
#include "ChessBoard.h"
#include "main.h"

int toAndFrom[4];

int main()
{
  chessboardWidth = 600;
  chessboardHeight = 600;
  squareSize = 1.0*chessboardHeight/8;

  Connector connector;
  connector.connectToEngine("stockfish.exe");

  ChessGame chessgame(false);
  ChessBoard chessboard = chessgame.getChessBoard();

  Coordinates oldCoordinates;
  Coordinates newCoordinates;

  std::string entireMoveHistory;
  sf::RenderWindow window(sf::VideoMode(600, 600), "Chess");

  sf::Vector2f oldPos;
  sf::Vector2f newPos;
  int n = 0;
  bool isMove = false;
  float dx = 0, dy = 0;
  while (window.isOpen()) {
  	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
  	sf::Event e;
  	while (window.pollEvent(e)) {
  	  if (e.type == sf::Event::Closed)
  	  	window.close();
  	  if (e.type == sf::Event::MouseButtonPressed) {
  	  	if (e.key.code == sf::Mouse::Left) {
  	  		for(int i = 0; i < 32; ++i){
  	  		  if (chessboard.getChessSprites()[i].getGlobalBounds().contains(mousePosition.x,mousePosition.y)) {
  	  			isMove=true; n=i;
  	  			dx = mousePosition.x - chessboard.getChessSprites()[i].getPosition().x;
  	  			dy = mousePosition.y - chessboard.getChessSprites()[i].getPosition().y;
  	  			oldPos = chessboard.getChessSprites()[i].getPosition();
  	  		 }
  	  		}
  	  	}
  	  }
  	  if (e.type == sf::Event::MouseButtonReleased) {
  	  	if (e.key.code == sf::Mouse::Left) {
  	  	  isMove = false;
  	  	  sf::Vector2f p = chessboard.getChessSprites()[n].getPosition()+
  	  	    sf::Vector2f(chessboard.getChessBoardSquareSize()/2,chessboard.getChessBoardSquareSize()/2);
  	  	  newCoordinates.setCoords(p);
          newPos = newCoordinates.getPixelCoords();
          int* dest = newCoordinates.getIntegerCoords();
          int* start = oldCoordinates.getIntegerCoords();

          if (chessboard.movePiece(chessgame.getTurn(),start[1], start[0], dest[1], dest[0])) {
            chessboard.move(oldCoordinates.getPixelCoords(), newCoordinates.getPixelCoords());
            entireMoveHistory += oldCoordinates.getRankFileIndex() + newCoordinates.getRankFileIndex() + " ";
  	  	    chessboard.getChessSprites()[n].setPosition(newPos);
            chessgame.nextTurn();
  	  	} else {
          //failed move
          chessboard.getChessSprites()[n].setPosition(sf::Vector2f(oldPos.x,oldPos.y));
        }
    }
  }
    //computer moves
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
      if (chessgame.getTurn() != "BLACK" || chessgame.isHumanOpponent()) {
        continue;
      }
      std::string compMove = connector.getCompMove(entireMoveHistory);
      oldCoordinates.setCoords(compMove.substr(0,2));
      newCoordinates.setCoords(compMove.substr(2,2));
      chessboard.movePiece(chessgame.getTurn(), oldCoordinates.getIntegerCoords()[1], oldCoordinates.getIntegerCoords()[0],
                            oldCoordinates.getIntegerCoords()[3], oldCoordinates.getIntegerCoords()[2]);

      for (int i=0;i<32;i++)
      if (chessboard.getChessSprites()[i].getPosition()==oldCoordinates.getPixelCoords()) {
        n=i;
      }
      move(compMove);
      entireMoveHistory += compMove + " ";
      chessboard.getChessSprites()[n].setPosition(newCoordinates.getPixelCoords());
      chessgame.nextTurn();
    }
    }
    if (isMove) {
      chessboard.getChessSprites()[n].setPosition(mousePosition.x-dx,mousePosition.y-dy);
    }
    window.clear();
    window.draw(chessboard.getBoardSprite());
    for (int i = 0; i < 32; ++i) {
        window.draw(chessboard.getChessSprites()[i]);
    }
    window.display();
  }
  return 0;
}
