#include "DrillScreen.h"

#include "CheckMateScreen.h"
#include "GameScreen.h"
#include "global.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>

DrillScreen::DrillScreen()
{
  canReverseMoves = true;
  connector.connectToEngine((char *)"stockfish.exe");
  chessgame.setPlayer2Human(false);
  chessboard = chessgame.getChessBoard();
  loadScreen();
}

int DrillScreen::Run(sf::RenderWindow &App)
{
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
        case sf::Event::MouseWheelScrolled: {
          chessgame.reverseMove();
          loadPosition(App);
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
          if (retry.getGlobalBounds().contains(mousePosF))
          {
            retry.setColor(sf::Color(220, 220, 220));
          } else {
            retry.setColor(sf::Color(0, 0, 0));
          }
          for (size_t i = 0; i < rectangle.size(); ++i) {
            if (rectangle[i].getGlobalBounds().contains(mousePosF))
            {
              rectangle[i].setFillColor(sf::Color(220, 220, 220));
            } else {
              rectangle[i].setFillColor(sf::Color(241, 241, 241));
            }
  	      }
          for (size_t i = 0; i < drillType.size(); i++) {
            if (drillType[i].getGlobalBounds().contains(mousePosF)) {
              drillType[i].setFillColor(sf::Color(128,128,128));
            } else {
              drillType[i].setFillColor(sf::Color(105,105,105));
            }
          }
          break;
  	    }
  	    case sf::Event::MouseButtonPressed : {
          if (isPressed) {
            chesspieceSprite[touchedByPlayer].setPosition(oldPos.x,oldPos.y);
            isPressed = false;
  	      }
          if (e.key.code == sf::Mouse::Left) {
            if (backButton.getGlobalBounds().contains(mousePosF)) {
              chessgame.reverseMove();
              loadPosition(App);
              buttonPressed = true;
            }
          for (size_t i = 0; i < drillType.size(); i++) {
            if (drillType[i].getGlobalBounds().contains(mousePosF)) {
              buttonPressed = true;
              currentDrill = i;
              chessgame.loadSavedGame("data/drills/" + drill[i]);
              loadPosition(App);
            }
          }
          if (mainMenu.getGlobalBounds().contains(mousePosF)) {
              return 0;
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
            if (retry.getGlobalBounds().contains(mousePosF))
            {
              touchedByPlayer = -1;
              chessgame.loadSavedGame("data/drills/" + drill[currentDrill]);
              loadPosition(App);
            }
            buttonPressed = false;
          }
          break;
        }

      }
      if (isPressed) {
        chesspieceSprite[touchedByPlayer].setPosition(mousePosition.x-dx,
                                                      mousePosition.y-dy);
      }
      refreshDisplay(App);

      if (chessgame.getTurn() == "BLACK" && !chessgame.isHumanOpponent()
          && !chessgame.hasFinished()) {
        computerMove();
      }
    }
  }
  return -1;
}

void DrillScreen::refreshDisplay(sf::RenderWindow &App) {
  App.clear();
  App.draw(background);
  App.draw(sidePanel);
  App.draw(bottomPanel);
  App.draw(boardSprite);
  for (size_t i = 0; i < rectangle.size(); ++i) {
    App.draw(rectangle[i]);
  }
  App.draw(mainMenu);
  App.draw(retry);
  for (size_t i = 0; i < chesspieceSprite.size(); ++i) {
      App.draw(chesspieceSprite[i]);
  }
  for (size_t i = 0; i < drillType.size(); ++i) {
      App.draw(drillType[i]);
  }
  App.draw(backButton);
  App.draw(forwardButton);
  App.display();
}

void DrillScreen::loadScreen()
{
  sidePanel_t.loadFromFile("images/sidePanel.png");
  sidePanel.setTexture(sidePanel_t);
  sidePanel.setPosition({600.f, g_pixel_dy});

  bottomPanel_t.loadFromFile("images/Bottom Panel.png");
  bottomPanel.setTexture(bottomPanel_t);
  bottomPanel.setPosition({g_pixel_dx, g_pixel_dy + 555.f});
  if (!Font.loadFromFile("images/verdanab.ttf"))
	{
		std::cerr << "Error loading verdanab.ttf" << std::endl;
	}

	for (size_t i = 0; i < drillType.size(); i++) {
    drillType[i].setFont(Font);
    drillType[i].setString(drill[i]);
    drillType[i].setCharacterSize(16);
    drillType[i].setPosition({605.f, 165.f + i*45});
    drillType[i].setFillColor(sf::Color(105,105,105));
	}
  backButton_t.loadFromFile("images/backButton.png");
  backButton.setTexture(backButton_t);
  backButton.setPosition(190 + g_pixel_dx, 555 + g_pixel_dy);

  forwardButton_t.loadFromFile("images/forwardButton.png");
  forwardButton.setTexture(forwardButton_t);
  forwardButton.setPosition(295 + g_pixel_dx, 555 + g_pixel_dy);

  loadChessSprites();
  loadSounds();

}
