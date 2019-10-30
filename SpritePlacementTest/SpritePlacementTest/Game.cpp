#include "Game.h"

Game::Game() : m_window(sf::VideoMode(800, 600), "YAMLPlacer Test")
{
	if (!m_textFont.loadFromFile("./resources/fonts/arial.ttf"))
	{
		std::string errorMsg("Error loading texture");
		throw std::exception(errorMsg.c_str());
	}

	m_typeText.setFont(m_textFont);
	m_typeText.setCharacterSize(16u);
	m_typeText.setString("WALL");
}

Game::~Game()
{
}

void Game::run()
{
	while (m_window.isOpen())
	{
		update();
		draw();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			obstaclePlacer.pickFile();
			obstaclePlacer.saveToFile();
			break;
		}
	}
}

void Game::update()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Q)
			{
				obstaclePlacer.decreaseRotation();
			}

			if (event.key.code == sf::Keyboard::E)
			{
				obstaclePlacer.increaseRotation();
			}

			if (event.key.code == sf::Keyboard::Z)
			{
				obstaclePlacer.undoObstacle();
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.key.code == sf::Mouse::Left)
			{
				obstaclePlacer.placeObstacle(m_window);
			}
		}
	}

	obstaclePlacer.pickObstacle();
}

void Game::draw()
{
	m_window.clear();

	if (obstaclePlacer.getType() == YAMLPlacer::ObstacleType::WALL)
		m_typeText.setString("Current Type: WALL");
	else if(obstaclePlacer.getType() == YAMLPlacer::ObstacleType::TARGET)
		m_typeText.setString("Current Type: TARGET");

	obstaclePlacer.showPreview(m_window);
	m_window.draw(m_typeText);
	m_window.display();
}
