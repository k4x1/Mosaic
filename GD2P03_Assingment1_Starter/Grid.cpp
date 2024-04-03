#include "Grid.h"

void Grid::InitGrid()
{
	m_grid.resize(m_gridSize, std::vector<FileImages>(m_gridSize));
	int x = 0;
	int y = 0;
	for (int i = 0; i < m_gridSize; i++) {

		for (int j = 0; j < m_gridSize; j++) {
		
			m_grid[i][j].m_empty = true;
			m_grid[i][j].m_image.setTexture(&m_emptyTexure,false);
			m_grid[i][j].m_image.setPosition(m_tileSize.x*i, m_tileSize.y*j);
			m_grid[i][j].m_image.setSize(m_tileSize);
		}
	}
	std::cout << x << " | " << y << std::endl;
}

Grid::Grid()
{
	m_gridSize = 4;
	m_gridCount = 0;
	m_tileSize = sf::Vector2f(900/m_gridSize, 900/m_gridSize);
	m_emptyTexure.loadFromFile("Images/emptyTile.png");
}

Grid::~Grid()
{
}
