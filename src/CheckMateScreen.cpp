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
        case sf::Event::MouseWheelScrolled: {
          break;
  	    }
        case sf::Event::MouseButtonPressed: {
          break;
  	    }
        case sf::Event::MouseButtonReleased: {
          break;
  	    }
        case sf::Event::MouseMoved: {
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
      }
    }
  window.draw(sprite);
  window.display();

  }
  return -1;
}

