#pragma once
#include "FileImages.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
class Grid
{
public:
	std::vector<std::vector<FileImages>> m_grid;
	sf::Vector2f m_tileSize;
	int m_gridSize;
	//size of grid
	sf::Texture m_emptyTexure;
	int m_gridCount;

	void setTileTexture(int _x, int _y, sf::Texture _texture);
	//sets the texture of a tile in that position;
	void addTile(FileImages tile);
	void addRowCol();
	void InitGrid();
	//returns tile added position, automatically calculates where this tile should go
	Grid();
	~Grid();
};
