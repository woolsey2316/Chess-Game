#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <time.h>
#include <string>
#include <Screens.h>

int main()
{
  std::vector<cScreen*> Screens;
	int screen = 0;

  sf::RenderWindow App(sf::VideoMode(950, 650), "Chess", sf::Style::Close);

  MainScreen s1;
	Screens.push_back(&s1);
	GameScreen s2;
	Screens.push_back(&s2);
  DrillScreen s3;
  Screens.push_back(&s3);
  AnalysisScreen s4;
  sf::Thread thread(&AnalysisScreen::timer, &s4);
  thread.launch();
  Screens.push_back(&s4);
	while (screen >= 0)
	{
		screen = Screens[screen]->Run(App);
	}
	return EXIT_SUCCESS;
}
