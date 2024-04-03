#pragma once
#include <string.h>
#include <iostream>
#include <SFML/Graphics.hpp>
class FileImages
{
public:
	std::string m_filePath;
	bool m_rendered;
	bool m_empty;
	sf::RectangleShape m_image;
	//	sf::Texture m_texture;
	FileImages();
	~FileImages();
};
