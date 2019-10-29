#include "YAMLPlacer.h"

YAMLPlacer::YAMLPlacer()
{
	
	if (!m_texture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string errorMsg("Error loading texture");
		throw std::exception(errorMsg.c_str());
	}

	m_obstaclePreview.setTexture(m_texture); // set the texture
	m_obstaclePreview.setTextureRect(m_wallRect); // change the texture rectangle to show only the wall obstacle
	m_obstaclePreview.setOrigin(m_wallRect.width / 2.0f, m_wallRect.height / 2.0f); // set the origin of the sprite
	m_obstaclePreview.setColor(sf::Color{ 255,255,255,100 }); // make the preview have less transparency


	// set up texture and sprite for sprite checking later
	m_currentObstacle.setTexture(m_texture);
	m_currentObstacle.setTextureRect(m_wallRect);
	m_currentObstacle.setOrigin(m_wallRect.width / 2.0f, m_wallRect.height / 2.0f);

	m_previousObstacle.setTexture(m_texture);
	m_previousObstacle.setTextureRect(m_wallRect);
	m_previousObstacle.setOrigin(m_wallRect.width / 2.0f, m_wallRect.height / 2.0f);
}

/// <summary>
/// Unused Destructor, but needed to be defined
/// </summary>
YAMLPlacer::~YAMLPlacer()
{
}

/// <summary>
/// Show a Preview of the currently selected Sprite type at the Player's mouse,
/// to show what the Sprite will look like when placed.
/// 
/// Also show a Preview of placed Sprites so the Player knows where they have placed them.
/// </summary>
/// <param name="t_window">Window to draw sprites</param>
void YAMLPlacer::showPreview(sf::RenderWindow& t_window)
{
	m_obstaclePreview.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(t_window)));
	m_obstaclePreview.setRotation(m_previewRotation);
	t_window.draw(m_obstaclePreview);

	for (size_t index = 0; index < m_positions.size(); index++)
	{
		m_obstaclePreview.setPosition(m_positions[index]);
		m_obstaclePreview.setRotation(m_rotations[index]);
		t_window.draw(m_obstaclePreview);
	}
}

/// <summary>
/// Place obstacles on Mouse click, doesn't allow for overlapping obstacles.
/// </summary>
/// <param name="t_window">Window so we know where the User is clicking</param>
void YAMLPlacer::placeObstacle(sf::RenderWindow& t_window)
{
	// First we need to make sure that the place where the Player is attempting to place an obstacle
	// isn't where another obstacle is (so we have no overlapping obstacles)

	bool isOverlapping{ false };
	m_currentObstacle.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(t_window)));
	m_currentObstacle.setRotation(m_previewRotation);

	sf::Sprite * spriteReference = new sf::Sprite{ m_previousObstacle }; 
	// use a pointer so we dont have to compare a bunch of sprites

	for (size_t i = 0; i < m_positions.size(); i++)
	{
		spriteReference->setPosition(m_positions[i]);
		spriteReference->setRotation(m_rotations[i]);

		if (m_currentObstacle.getGlobalBounds().intersects(spriteReference->getGlobalBounds()))
		{
			isOverlapping = true;
			break;
		}
	}

	delete spriteReference;
	
	if (!isOverlapping)
	{ // if the obstacle the user is trying to place does not intersect any other obstacles, add it to the vector
		m_positions.push_back(static_cast<sf::Vector2f>(sf::Mouse::getPosition(t_window)));
		m_rotations.push_back(m_previewRotation);

		if (m_previewType == WALL)
			m_types.push_back(WALL);
		else
			m_types.push_back(TARGET);
	}
#ifdef _DEBUG // only show this message if we're in debug mode
	else
	{
		std::cout << "User attempted to place Obstacle ontop of another one!" << std::endl << "Try again somewhere else." << std::endl;	
	}
#endif // _DEBUG
}

/// <summary>
/// Get the User to choose a File to save this information to.
/// If the file already exists, the User will be told so.
/// </summary>
void YAMLPlacer::pickFile()
{
	int choice = 0;

	while (true)
	{
		std::cout << "Please choose a file: ";
		std::cin >> m_levelNum;

		std::fstream file("resources/levels/level" + std::to_string(m_levelNum) + ".yaml");

		if (file)
		{
			std::cout << "File already exists! Data may be added ontop of existing data." << std::endl;
			std::cout << "Is this okay? (0 - NO, Any other Number for YES)";
			std::cin >> choice;

			if (choice == 0)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	
}

/// <summary>
/// We check to see multiple scenarios here:
/// 1. If the file does not exist - make it and fill it with information.
/// 2. If the file does exist, but obstacle does not, make an obstacle tab and fill information underneath.
/// 3. If the file exists, AND obstacle exists, fill information underneath obstacle.
/// </summary>
void YAMLPlacer::saveToFile()
{
	std::string line; // current line
	std::string lineContents; // holds information that we may need
	std::fstream fileToUse; // file we're reading from (same as file we are saving to)
	int letterPosition = -1; // the position where the information we may need is
	bool foundObstacles = false;

	fileToUse.open("resources/levels/level" + std::to_string(m_levelNum) + ".yaml", std::ios::in | std::ios::app);


	if (fileToUse.is_open()) // if we can read the file
	{
		if (fileToUse.peek() == std::ios::traits_type::eof())
		{ // if the file has nothing in it
		  // fill all the information we need into the file

			fileToUse.close(); // close the file since we want to write to it now
			fileToUse.open("resources/levels/level" + std::to_string(m_levelNum) + ".yaml", std::ios::out | std::ios::app);
			std::cout << "End of file found early, assuming empty, creating file from scratch..." << std::endl;
			fileToUse << "background: " << std::endl;
			fileToUse << "   file: ./resources/images/Background.jpg # Sample comment" << std::endl;
			fileToUse << "tank:" << std::endl;
			fileToUse << "   position: {x: 800, y: 100}" << std::endl;
			fileToUse << "   max_projectiles: 10" << std::endl;
			fileToUse << "   reload_time: 1000  " << std::endl;
			fileToUse << "ai_tank:" << std::endl;
			fileToUse << "   position: {x: 400, y: 700}" << std::endl;
			fileToUse << "   max_projectiles: 10" << std::endl;
			fileToUse << "   reload_time: 1000  " << std::endl;
			fileToUse << "projectile:" << std::endl;
			fileToUse << "   speed: 1000" << std::endl;
			fileToUse << "   damage: 10	" << std::endl;
			fileToUse << "obstacles:" << std::endl;

			constructYAML(fileToUse);
		}
		else
		{
			while (std::getline(fileToUse, line)) // while there's lines to go through
			{
				letterPosition = line.find('o');
				if (letterPosition >= 0)
					lineContents = line.substr(letterPosition);

				// get the contents AFTER the =, as that's the data we need

				if (lineContents == "obstacles:")
				{ // if the line we are at is obstacles
					std::cout << "Found obstacles:";
					foundObstacles = true;

					fileToUse.close(); // close the file since we want to write to it now
					fileToUse.open("resources/levels/level" + std::to_string(m_levelNum) + ".yaml", std::ios::app);

					fileToUse << std::endl;

					constructYAML(fileToUse);

					break;
				}
			}

			if (!foundObstacles)
			{ // if the word obstacles was not found
			  // we will add the obstacles ourselves
				fileToUse.close(); // close the file since we want to write to it now
				fileToUse.open("resources/levels/level" + std::to_string(m_levelNum) + ".yaml", std::ios::app);

				fileToUse << std::endl << "obstacles:" << std::endl;

				constructYAML(fileToUse);
			}
		}
	}
	else
	{
		std::cerr << "Error opening level " << m_levelNum << ", make sure it's in the right directory!";
	}

	fileToUse.close();
}

void YAMLPlacer::constructYAML(std::fstream & m_fileToUse)
{
	/*
	Two for loops are used here so we can have seperate data.
	The result will look like:
	Wall:
	Wall:
	...
	Target:
	Target
	*/

	for (size_t index = 0; index < m_types.size(); index++)
	{
		if (m_types[index] == WALL)
		{
			m_fileToUse << "   - type: wall" << std::endl;
			m_fileToUse << "     position: {x: " << m_positions[index].x << ", y: " << m_positions[index].y << "}" << std::endl;
			m_fileToUse << "     rotation: " << m_rotations[index];
			if (index < m_positions.size() - 1)
				m_fileToUse << std::endl; // only make a new line if there is more data to process
		}	
	}

	for (size_t index = 0; index < m_types.size(); index++)
	{
		if (m_types[index] == TARGET)
		{
			m_fileToUse << "   - type: target" << std::endl;
			m_fileToUse << "     position: {x: " << m_positions[index].x << ", y: " << m_positions[index].y << "}" << std::endl;
			m_fileToUse << "     randomOffset: 20";
			if (index < m_positions.size() - 1)
				m_fileToUse << std::endl; // only make a new line if there is more data to process
		}
	}
}

/// <summary>
/// Increase rotation of our Preview sprite and wrap around at 360
/// </summary>
void YAMLPlacer::increaseRotation()
{
	m_previewRotation += 1;
	if (m_previewRotation == 360.0)
	{
		m_previewRotation = 0;
	}
}

/// <summary>
/// Decrease rotation of our Preview sprite and wrap back around at 0
/// </summary>
void YAMLPlacer::decreaseRotation()
{
	m_previewRotation -= 1;
	if (m_previewRotation == 0.0)
	{
		m_previewRotation = 359.0;
	}
}

/// <summary>
/// Let the user choose obstacles using Num1 or Num2
/// </summary>
void YAMLPlacer::pickObstacle()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		m_previewType = WALL;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		m_previewType = TARGET;
	}
}

/// <summary>
/// Get the current obstacle type that the preview is showing
/// </summary>
/// <returns>Current type for preview</returns>
YAMLPlacer::ObstacleType YAMLPlacer::getType() const
{
	return m_previewType;
}
