#include "GameScreen.h"
#include "global.h"
#include "CheckMateScreen.h"
#include "LayoutScreen.h"
#include "OptionMenu.h"

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
          if (isPressed) {
            chesspieceSprite[touchedByPlayer].setPosition(oldPos.x,oldPos.y);
            isPressed = false;
  	      }
          if (e.key.code == sf::Mouse::Left) {
            if (settings.getGlobalBounds().contains(mousePosF)) {
              LayoutScreen layoutScreen(chessBoardFile);
              setChessBoardFile(layoutScreen.getChosenImageFile());
              buttonPressed = true;
            }
            if (computerDifficulty.getGlobalBounds().contains(mousePosF)) {
              OptionMenu optionMenu(chessgame.getComputer().getDifficulty());
              int difficulty = optionMenu.getChosenDifficulty();
              chessgame.getComputer().setDifficulty(difficulty);
              computerDifficulty.setString("Computer Level " + std::to_string(difficulty));
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
  for (size_t i = 0; i < chesspieceSprite.size(); ++i) {
      App.draw(chesspieceSprite[i]);
  }
  App.draw(computerDifficulty);
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

  if (!Font.loadFromFile("images/verdanab.ttf"))
	{
		std::cerr << "Error loading verdanab.ttf" << std::endl;
	}

  computerDifficulty.setFont(Font);
  computerDifficulty.setString("Computer Level " + std::to_string(chessgame.getComputer().getDifficulty()));
  computerDifficulty.setCharacterSize(18);
  computerDifficulty.setPosition({700, g_pixel_dy + 50});
  computerDifficulty.setFillColor(sf::Color::Blue);
  computerDifficulty.getLocalBounds();

  loadChessSprites();
  loadSounds();
}


