#include "CheckMateScreen.h"

CheckMateScreen::CheckMateScreen()
{
  sf::RenderWindow window(sf::VideoMode(854, 444), "CheckMate");

  row = (isWhite ? 0 : 7);
  texture.loadFromFile("images/checkmate.jpg");
  sprite.setTexture(texture);

  Run(window);

}

int CheckMateScreen::Run(sf::RenderWindow &window)
{
  bool running = true;
  while (running) {
  	mousePosition = sf::Mouse::getPosition(window);
  	sf::Vector2f mousePosF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
  	sf::Event e;
  	while (window.pollEvent(e)) {
      switch (e.type) {
        case sf::Event::Closed: {
  	  	  window.close();
  	  	  return -1;
  	  	  break;
  	    }
      }
    }
  window.draw(sprite);
  window.display();

  }
  return -1;
}

