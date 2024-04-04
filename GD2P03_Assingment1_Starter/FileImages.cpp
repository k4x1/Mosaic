#include "FileImages.h"

void FileImages::setTexture(sf::Texture* _texture)
{
	m_empty = false;
	m_image.setTexture(_texture, true);
}

FileImages::FileImages()
{
	m_rendered = false;
	m_filePath = "";
	m_empty = true;

	//m_empty = (m_filePath == "");	



}

FileImages::~FileImages()
{
}
