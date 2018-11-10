#include "LayoutScreen.h"

LayoutScreen::LayoutScreen()
{
  sf::RenderWindow window(sf::VideoMode(854, 444), "Set Layout");

  for (int i = 0; i < 4; ++i) {
    chessboard_t[i].loadFromFile("images/chessboard" + std::to_string(i) + ".png");
    chessboardSprite[i].setTexture(chessboard_t[i]);
    chessboardSprite[i].scale(0.15, 0.15);
    chessboardSprite[i].setPosition(100*i, 10);

    rectangle[i].setSize(sf::Vector2f(100, 100));
    rectangle[i].setFillColor(sf::Color(200,200,200));
    rectangle[i].setSize(sf::Vector2f(100, 100));
    rectangle[i].setPosition(40*i, 10);


  }

  Run(window);

}

int LayoutScreen::Run(sf::RenderWindow &window)
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
  	  	  return -1;
  	  	  break;
  	    }
      }
    }
    for (int i = 0; i < 4; ++i) {
      window.draw(chessboardSprite[i]);
      window.draw(rectangle[i]);
    }
    window.display();

  }
  return -1;
}

