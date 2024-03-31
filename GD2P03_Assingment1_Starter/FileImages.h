#pragma once
#include <string.h>
#include <iostream>
#include <SFML/Graphics.hpp>
class FileImages
{
	public:
		std::string filePath;
		bool rendered;
		std::string url;
		sf::Vector2i Position;
		sf::Vector2f size;
		std::vector<sf::RectangleShape> image;
		FileImages();
		~FileImages();

};

