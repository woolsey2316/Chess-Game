#include "OptionMenu.h"
#include <string>

OptionMenu::OptionMenu(int difficulty_)
{
  difficulty = difficulty_;
  sf::RenderWindow window(sf::VideoMode(328, 322), "Set Layout", sf::Style::Close);
  window.setFramerateLimit(60);
  background_t.loadFromFile("images/DifficultySetting.png");
  background.setTexture(background_t);
  sf::Font font;
  if (!font.loadFromFile("images/verdanab.ttf"))
      return;

  for (size_t i = 0; i < 10; ++i) {
    sf::CircleShape circle;
    circle.setRadius(15);
    circle.setFillColor(sf::Color(105,105,105));
    circle.setPosition(35*i+15,120);
    circles[i] = circle;

  }
  for (size_t i = 1; i < 11; ++i) {
    sf::Text text(std::to_string(i), font);
    text.setCharacterSize(11);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::White);
    text.setPosition(35*(i-1)+25,130);
    number[i-1] = text;
  }
  Run(window);
}

int OptionMenu::Run(sf::RenderWindow &window)
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
  	  	  return 4;
  	  	  break;
  	    }
  	    case sf::Event::MouseMoved: {
  	      for (size_t i = 0; i < circles.size(); ++i) {
            if (circles[i].getGlobalBounds().contains(mousePosF))
            {
              circles[i].setFillColor(sf::Color(255,69,0));
            } else {
              circles[i].setFillColor(sf::Color(105,105,105));
            }
  	      }
  	      break;
        }
        case sf::Event::MouseButtonPressed: {
          for (size_t i = 0; i < circles.size(); i++) {
            if (circles[i].getGlobalBounds().contains(mousePosF)) {
              difficulty = i;
              window.close();
              return difficulty;
            }
          }
          break;
        }
        case sf::Event::MouseButtonReleased: {
          break;
        }
      }
      window.draw(background);
      for (size_t i = 0; i < circles.size(); ++i) {
        window.draw(circles[i]);
      }
      for (size_t i = 0; i < number.size(); ++i) {
        window.draw(number[i]);
      }
      window.display();

    }
  }
  return -1;
}

int OptionMenu::getChosenDifficulty() {
  return difficulty;
}

void OptionMenu::setChosenDifficulty(int difficulty_) {
  if (difficulty != -1) {
    difficulty = difficulty_;
  }
}

