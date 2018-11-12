#include "MainScreen.h"


MainScreen::MainScreen(void)
{
	alpha_max = 3 * 255;
	alpha_div = 3;
	playing = false;
}

int MainScreen::Run(sf::RenderWindow &App)
{
	sf::Event Event;
	bool Running = true;
	sf::Texture Texture;
	sf::Sprite Sprite;
	sf::Font Font;
	sf::Text Menu1;
	sf::Text Menu2;
	sf::Text Menu3;
  sf::Text Menu4;
  sf::Text Menu5;
  int alpha = 0;
	int menu = -1;

	if (!Texture.loadFromFile("images/Main Screen.jpg"))
	{
		std::cerr << "Error loading Main Screen" << std::endl;
		return (-1);
	}
	if (!Font.loadFromFile("images/verdanab.ttf"))
	{
		std::cerr << "Error loading verdanab.ttf" << std::endl;
		return (-1);
	}
	Sprite.setTexture(Texture);
	Sprite.setColor(sf::Color(255, 255, 255, alpha));

	Menu1.setFont(Font);
	Menu1.setCharacterSize(20);
	Menu1.setString("New Game");
	Menu1.setPosition({280.f, 160.f});

	Menu2.setFont(Font);
	Menu2.setCharacterSize(20);
	Menu2.setString("Checkmate Drills");
	Menu2.setPosition({280.f, 220.f});

	Menu3.setFont(Font);
	Menu3.setCharacterSize(20);
	Menu3.setString("Continue");
	Menu3.setPosition({280.f, 160.f});

  Menu4.setFont(Font);
	Menu4.setCharacterSize(20);
	Menu4.setString("Analysis");
	Menu4.setPosition({280.f, 280.f});

  Menu5.setFont(Font);
	Menu5.setCharacterSize(20);
	Menu5.setString("Exit");
	Menu5.setPosition({280.f, 340.f});

	while (Running)
	{
	  sf::Vector2i mousePosition = sf::Mouse::getPosition(App);
  	sf::Vector2f mousePosF(static_cast<float>(mousePosition.x), static_cast<float>( mousePosition.y ));
		while (App.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}
			if (Event.type == sf::Event::MouseMoved)
			{
				if (Menu1.getGlobalBounds().contains(mousePosF)) {
				  Menu1.setColor(sf::Color(250, 20, 20));
				} else {
				  Menu1.setColor(sf::Color(250, 255, 255));
				}
				if (Menu2.getGlobalBounds().contains(mousePosF)) {
				  Menu2.setColor(sf::Color(250, 20, 20));
				} else {
				  Menu2.setColor(sf::Color(250, 255, 255));
				}
				if (Menu3.getGlobalBounds().contains(mousePosF)) {
				  Menu3.setColor(sf::Color(250, 20, 20));
				} else {
				  Menu3.setColor(sf::Color(250, 255, 255));
				}
				if (Menu4.getGlobalBounds().contains(mousePosF)) {
				  Menu4.setColor(sf::Color(250, 20, 20));
				} else {
				  Menu4.setColor(sf::Color(250, 255, 255));
				}
        if (Menu5.getGlobalBounds().contains(mousePosF)) {
				  Menu5.setColor(sf::Color(250, 20, 20));
				} else {
				  Menu5.setColor(sf::Color(250, 255, 255));
				}
			}
			if (Event.type == sf::Event::MouseButtonPressed) {
        if (Menu1.getGlobalBounds().contains(mousePosF)) {
				  menu = 0;
				} else if (Menu2.getGlobalBounds().contains(mousePosF)) {
				  menu = 1;
				} else if (Menu3.getGlobalBounds().contains(mousePosF)) {
				  menu = 2;
				} else if (Menu4.getGlobalBounds().contains(mousePosF)) {
				  menu = 3;
				} else if (Menu5.getGlobalBounds().contains(mousePosF)) {
				  menu = 4;
				}
				if (menu == 0) {
				  playing = true;
				  return (1);
				} else if (menu == 1) {
				  return (2);
				} else if (menu == 2) {
          return (3);
				} else if (menu == 3) {
          return (3);
				} else if (menu == 4) {
          return (-1);
				}
			}
		}
		if (alpha<alpha_max) {
			alpha++;
		}
		Sprite.setColor(sf::Color(255, 255, 255, alpha / alpha_div));

  //Clearing screen
  App.clear();
  //Drawing
  App.draw(Sprite);
  if (alpha == alpha_max)
  {
    if (playing)
    {
      App.draw(Menu3);
    }
    else
    {
      App.draw(Menu1);
    }
    App.draw(Menu2);
    App.draw(Menu4);
    App.draw(Menu5);
  }
  App.display();
}

	return (-1);
}
