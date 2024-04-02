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
		sf::Vector2f TileSize;
		sf::Texture txt;
		
		void setTileTexture(int _x, int _y, sf::Texture _texture);
		//sets the texture of a tile in that position;
		sf::Vector2i addTile(FileImages tile);
		sf::Vector2i addRowCol(); 
		void SetupImages(float _size);
		//returns tile added position, automatically calculates where this tile should go
		Grid();
		~Grid();
};

