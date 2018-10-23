#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H


#include <iostream>
#include "cScreen.h"

#include <SFML/Graphics.hpp>

class SettingsScreen : public cScreen
{
private:
	int alpha_max;
	int alpha_div;
	bool playing;
public:
	SettingsScreen(void);
	virtual int Run(sf::RenderWindow &App);
};

#endif // SETTINGSSCREEN_H
