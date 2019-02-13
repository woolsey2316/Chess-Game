#include "AnalysisScreen.h"

#include "CheckMateScreen.h"
#include "GameScreen.h"
#include "global.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>
#include <map>

AnalysisScreen::AnalysisScreen()
{
  canReverseMoves = true;
  connector.connectToEngine((char *)"stockfish.exe");
  //computer ordinary match
  chessgame.setPlayer2Human(true);
  chessboard = chessgame.getChessBoard();
  loadScreen();
}

int AnalysisScreen::Run(sf::RenderWindow &App)
{
  loadPosition(App);
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
        case sf::Event::LostFocus: {
          break;
  	    }
        case sf::Event::TouchEnded: {
          break;
  	    }
        case sf::Event::SensorChanged: {
          break;
  	    }
        case sf::Event::Count: {
          break;
  	    }
  	    case sf::Event::GainedFocus: {
          break;
  	    }
  	    case sf::Event::TextEntered: {
  	      break;
  	    }
        case sf::Event::KeyPressed: {
          break;
  	    }
        case sf::Event::KeyReleased: {
          break;
  	    }
        case sf::Event::MouseWheelMoved: {
          break;
  	    }
        case sf::Event::MouseWheelScrolled: {
          break;
  	    }
        case sf::Event::MouseEntered: {
          break;
  	    }
        case sf::Event::MouseLeft: {
          break;
  	    }
  	    case sf::Event::Resized: {
          break;
  	    }
  	    case sf::Event::JoystickButtonPressed: {
          break;
  	    }
  	    case sf::Event::JoystickButtonReleased: {
          break;
  	    }
  	    case sf::Event::JoystickMoved: {
          break;
  	    }
  	    case sf::Event::JoystickConnected: {
          break;
  	    }
  	    case sf::Event::JoystickDisconnected: {
          break;
  	    }
  	    case sf::Event::TouchBegan: {
          break;
  	    }
  	    case sf::Event::TouchMoved: {
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
          break;
  	    }
  	    case sf::Event::MouseButtonPressed : {
          if (e.key.code == sf::Mouse::Left) {
            if (backButton.getGlobalBounds().contains(mousePosF)) {
              reverseMove(App);
              buttonPressed = true;
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
              getPositionScores();
            }
            if (retry.getGlobalBounds().contains(mousePosF))
            {
              resetGame();
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
  App.draw(background);
  App.draw(sidePanel);
  App.draw(bottomPanel);
  App.draw(boardSprite);
  App.draw(mainMenu);
  App.draw(retry);
  for (size_t i = 0; i < rectangle.size(); ++i) {
      App.draw(rectangle[i]);
  }
  for (int i = 0; i < 32; ++i) {
    App.draw(chesspieceSprite[i]);
  }
  for (const auto & move : variation) {
    App.draw(move);
  }
  App.draw(backButton);
  App.draw(forwardButton);
  App.display();
}

void AnalysisScreen::loadScreen()
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

  backButton_t.loadFromFile("images/backButton.png");
  backButton.setTexture(backButton_t);
  backButton.setPosition(190 + g_pixel_dx, 560 + g_pixel_dy);

  forwardButton_t.loadFromFile("images/forwardButton.png");
  forwardButton.setTexture(forwardButton_t);
  forwardButton.setPosition(295 + g_pixel_dx, 560 + g_pixel_dy);

  loadChessSprites();
  loadSounds();

}


