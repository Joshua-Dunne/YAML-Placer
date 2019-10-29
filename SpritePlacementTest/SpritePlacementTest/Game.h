#pragma once
#include <SFML/Graphics.hpp>
#include "YAMLPlacer.h"

class Game
{
public:
	Game();
	~Game();

private:
	YAMLPlacer obstaclePlacer;
	sf::RenderWindow m_window;

	sf::Font m_textFont;
	sf::Text m_typeText;

public:
	void run();
	void update();
	void draw();
};

