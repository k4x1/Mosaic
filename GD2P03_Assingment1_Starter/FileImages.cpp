#include "FileImages.h"

FileImages::FileImages(std::string _filePath)
{
	m_rendered = false;

	m_filePath = _filePath;

	m_empty = (m_filePath == "");

}

FileImages::~FileImages()
{
}
