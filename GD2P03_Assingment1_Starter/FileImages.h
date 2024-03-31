#pragma once
#include <string.h>
#include <iostream>
class FileImages
{
	public:
		std::string filePath;
		bool rendered;
		std::string url;
		int imageY;
		int imageX;
		std::vector<sf::RectangleShape> image;
		FileImages();
		~FileImages();

};

