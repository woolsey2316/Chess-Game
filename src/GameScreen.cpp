#include "GameScreen.h"
#include "global.h"
#include "CheckMateScreen.h"
#include "LayoutScreen.h"

#include <boost/algorithm/string.hpp>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>

GameScreen::GameScreen()
{
  canReverseMoves = false;
  connector.connectToEngine((char *)"stockfish.exe");
  chessgame.setPlayer2Human(true);
  chessboard = chessgame.getChessBoard();
  loadScreen();
}

int GameScreen::Run(sf::RenderWindow &App)
{

  bool running = true;
  while (running) {
  	mousePosition = sf::Mouse::getPosition(App);
  	sf::Vector2f mousePosF(static_cast<float>( mousePosition.x ),
                           static_cast<float>( mousePosition.y ));
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
  	    }
        case sf::Event::KeyPressed: {
  	    }
        case sf::Event::KeyReleased: {
  	    }
        case sf::Event::MouseWheelMoved: {
  	    }
        case sf::Event::MouseWheelScrolled: {
  	    }
        case sf::Event::MouseEntered: {
  	    }
        case sf::Event::MouseLeft: {
  	    }
  	    case sf::Event::MouseMoved: {
          if (settings.getGlobalBounds().contains(mousePosF))
          {
            settings.setColor(sf::Color(220, 220, 220));
          } else {
            settings.setColor(sf::Color(190, 190, 190));
          }
          if (mainMenu.getGlobalBounds().contains(mousePosF))
          {
            mainMenu.setColor(sf::Color(220, 220, 220));
          } else {
            mainMenu.setColor(sf::Color(0, 0, 0));
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
            if (settings.getGlobalBounds().contains(mousePosF)) {
              LayoutScreen layoutScreen(chessBoardFile);
              setChessBoardFile(layoutScreen.getChosenImageFile());
              buttonPressed = true;
            }
            if (mainMenu.getGlobalBounds().contains(mousePosF))
            {
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

void GameScreen::refreshDisplay(sf::RenderWindow &App) {
  App.clear();
  App.draw(background);
  App.draw(sidePanel);
  App.draw(boardSprite);
  App.draw(settings);
  for (size_t i = 0; i < rectangle.size(); ++i) {
      App.draw(rectangle[i]);
  }
  App.draw(mainMenu);
  App.draw(retry);
  for (size_t i = 0; i < 32; ++i) {
      App.draw(chesspieceSprite[i]);
  }
  App.display();
}

void GameScreen::loadScreen()
{
  sidePanel_t.loadFromFile("images/sidePanelGame.png");
  sidePanel.setTexture(sidePanel_t);
  sidePanel.setPosition({600.f, g_pixel_dy});

  settings_t.loadFromFile("images/settings.png");
  settings.setTexture(settings_t);
  settings.setPosition({605, g_pixel_dy + 5});

  loadChessSprites();
}


