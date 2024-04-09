#pragma once
#include "FileImages.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
class Grid
{
public:
	std::vector<std::vector<std::vector<FileImages>>> m_grid;
	//epic 3 dimensional array elements are: page, x, y
	sf::Vector2f m_tileSize;
	int m_gridSize;
	//size of grid
	sf::Texture m_emptyTexure;
	int m_gridCount;

	int m_pageCount;

	int m_currentTileX = 0;
	int m_currentTileY = 0;
	int m_currentTilePage = 0;
	void setTileTextures( sf::Texture* _texture);
	//sets the texture of a tile in that position;
	void InitGrid(int _gridCount);

	void ResizeGrid(int _newSize);
	//returns tile added position, automatically calculates where this tile should go
	Grid();
	~Grid();
};
