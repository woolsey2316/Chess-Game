#include "LayoutScreen.h"

LayoutScreen::LayoutScreen(std::string fileName_)
{
  fileName = fileName_;
  sf::RenderWindow window(sf::VideoMode(854, 444), "Set Layout");

  for (size_t i = 0; i < 4; ++i) {
    chessboard_t[i].loadFromFile("images/chessboard" + std::to_string(i) + ".png");
    chessboardSprite[i].setTexture(chessboard_t[i]);
    chessboardSprite[i].scale(0.15, 0.15);
    chessboardSprite[i].setPosition(120*i, 10);

    rectangle[i].setSize(sf::Vector2f(0.15*600+10, 0.15*600+10));
    rectangle[i].setFillColor(sf::Color(200,200,200));
    rectangle[i].setSize(sf::Vector2f(100, 100));
    rectangle[i].setPosition(120*i-5, 10-5);

  }
  Run(window);
}

std::string LayoutScreen::Run(sf::RenderWindow &window)
{
  bool running = true;
  while (running) {
  	mousePosition = sf::Mouse::getPosition(window);
  	sf::Vector2f mousePosF(static_cast<float>(mousePosition.x),
                          static_cast<float>(mousePosition.y));
  	sf::Event e;
  	while (window.pollEvent(e)) {
      switch (e.type) {
        case sf::Event::Closed: {
  	  	  window.close();
  	  	  return "";
  	  	  break;
  	    }
  	    case sf::Event::MouseMoved: {
  	      for (size_t i = 0; i < 4; ++i) {
            if (rectangle[i].getGlobalBounds().contains(mousePosF))
            {
              rectangle[i].setFillColor(sf::Color(255, 255, 255));
            } else {
              rectangle[i].setFillColor(sf::Color(200, 200, 200));
            }
  	      }
  	      break;
        }

        case sf::Event::MouseButtonPressed: {
          for (size_t i = 0; i < 4; i++) {
            if (chessboardSprite[i].getGlobalBounds().contains(mousePosF)) {
              fileName = "images/chessboard" + std::to_string(i) + ".png";
              window.close();
              return fileName;
            }
          }
          break;
        }
      }

    }
    for (size_t i = 0; i < 4; ++i) {
      window.draw(rectangle[i]);
      window.draw(chessboardSprite[i]);
    }
    window.display();

  }
  return "";
}

std::string LayoutScreen::getChosenImageFile() {
  return fileName;
}

std::string LayoutScreen::setChosenImageFile(std::string fileName_) {
  if (fileName != "") {
    fileName = fileName_;
  }
}

