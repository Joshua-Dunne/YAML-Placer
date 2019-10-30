#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

/// <summary>
/// YAML Placer by Joshua Dunne - C00241588 for Tank YAML Project
/// -------------------------------------------------------------
/// Start Date: 25/10/19
/// End Date: ???
/// Time Taken: ~8 Hours
/// -------------------------------------------------------------
/// YAML Obstacle Placer - Add this to your project and call it's functions to place obstacles!
/// Can place both Walls and Targets (Targets coming soon)
/// All you need to do is pass a reference to your window.
/// As it stands, you WILL need to have your obstacles: in your YAML file at the very end, otherwise this won't work.
/// </summary>


class YAMLPlacer
{
public:
	YAMLPlacer();
	~YAMLPlacer();

	enum ObstacleType { WALL, TARGET };

private:
	int m_levelNum{ 1 }; // what YAML level file we're editing

	std::vector<ObstacleType> m_types; // stores the type of obstacle (wall, target)
	std::vector<sf::Vector2f> m_positions; // stores positions for obstacles
	std::vector<float> m_rotations; // stores rotations for obstacles

	ObstacleType m_previewType{ WALL }; // which type of obstacle the user wants to place

	float m_previewRotation{ 0 }; // preview object's rotation to be stored

	sf::Sprite m_obstaclePreview; // shown where the mouse is with rotations applied

	// used to make sure no two obstacles are placed overlapping
	sf::Sprite m_currentObstacle;
	// used to make sure no two obstacles are placed overlapping
	sf::Sprite m_previousObstacle;

	sf::IntRect m_wallRect{ 2, 129, 33, 23 }; // place on texture map where walls are
	sf::IntRect m_targetRect{ 53, 129, 33, 23 };
	sf::Texture m_texture;

public:
	void showPreview(sf::RenderWindow& t_window);
	void placeObstacle(sf::RenderWindow& t_window);
	void pickFile();
	void saveToFile();
	void constructYAML(std::fstream& m_fileToUse);

	void increaseRotation();
	void decreaseRotation();

	void pickObstacle();
	void undoObstacle();
	ObstacleType getType() const;
};

