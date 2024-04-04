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

	void setTileTextures(int _page, int _x, int _y, sf::Texture* _texture);
	//sets the texture of a tile in that position;
	void InitGrid(int _gridCount);

	void ResizeGrid(int _newSize, int _gridCount);
	//returns tile added position, automatically calculates where this tile should go
	Grid();
	~Grid();
};
