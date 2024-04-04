#include "Grid.h"

void Grid::setTileTextures(int _page, int _x, int _y, sf::Texture* _texture)
{
	m_grid[_page][_x][_y].m_empty = false;
	m_grid[_page][_x][_y].m_image.setTexture(_texture, false);
}

void Grid::InitGrid(int _gridCount)
{
	m_gridCount = _gridCount;
	m_grid.resize(m_gridSize, std::vector<std::vector<FileImages>>(m_gridSize, std::vector<FileImages>(m_gridSize)));
	int pageCount = round((m_gridCount / (m_gridSize * m_gridSize) + 0.5f));
	std::cout << pageCount << std::endl;
	for (int page = 0; page < pageCount; page++) {
		for (int i = 0; i < m_gridSize; i++) {

			for (int j = 0; j < m_gridSize; j++) {

				m_grid[page][i][j].m_empty = true;
				m_grid[page][i][j].m_image.setTexture(&m_emptyTexure, true);
				m_grid[page][i][j].m_image.setPosition(m_tileSize.x * i, m_tileSize.y * j);
				m_grid[page][i][j].m_image.setSize(m_tileSize);
			}
		}
	}
}

void Grid::ResizeGrid(int _newSize, int _gridCount)
{
	m_gridCount = _gridCount;


	m_gridSize = _newSize;
	m_tileSize = sf::Vector2f(900 / m_gridSize, 900 / m_gridSize);
	

	m_grid.resize(m_gridSize, std::vector<std::vector<FileImages>>(m_gridSize, std::vector<FileImages>(m_gridSize)));
	int pageCount = round((m_gridCount / (m_gridSize * m_gridSize) + 0.5f));
	for (int page = 0; page < pageCount; page++) {
		for (int i = 0; i < m_gridSize; i++) {

			for (int j = 0; j < m_gridSize; j++) {

				m_grid[page][i][j].m_image.setPosition(m_tileSize.x * i, m_tileSize.y * j);
				m_grid[page][i][j].m_image.setSize(m_tileSize);
			}
		}
	}
}

Grid::Grid()
{
	m_gridSize = 3;
	m_gridCount = 9;
	m_tileSize = sf::Vector2f(900/m_gridSize, 900/m_gridSize);
	m_emptyTexure.loadFromFile("emptyTile.png");
}

Grid::~Grid()
{
}
