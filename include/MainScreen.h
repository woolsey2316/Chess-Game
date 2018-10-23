#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "cScreen.h"

class MainScreen : public cScreen
{
private:
	bool playing;
	int alpha_max;
	int alpha_div;
public:
	MainScreen(void);
	virtual int Run(sf::RenderWindow &App);
};

#endif
