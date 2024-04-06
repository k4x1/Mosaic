#include "Grid.h"
void Grid::setTileTextures(sf::Texture* _texture)
{
	// Check if the current page index is out of range
	if (m_currentTilePage >= m_grid.size()) {
		// Resize the grid to include the new page
	//	m_grid.resize(m_grid.size() + 1, std::vector<std::vector<FileImages>>(m_gridSize, std::vector<FileImages>(m_gridSize)));
	}

	// Set the texture for the current tile
	m_grid[m_currentTilePage][m_currentTileX][m_currentTileY].setTexture(_texture);
	m_currentTileX++;

	// Check if the current tile X index is out of range
	if (m_currentTileX >= m_gridSize) {
		m_currentTileY++;
		// Check if the current tile Y index is out of range
		if (m_currentTileY >= m_gridSize) {
			m_currentTilePage++;
			m_currentTileY = 0;
		}
		m_currentTileX = 0;
	}
}


void Grid::InitGrid(int _gridCount)
{
	m_gridCount = _gridCount;
	m_pageCount = round((m_gridCount / (m_gridSize * m_gridSize) + 0.5f));
	m_grid.resize(m_pageCount, std::vector<std::vector<FileImages>>(m_gridSize, std::vector<FileImages>(m_gridSize)));
	
	
	for (int page = 0; page < m_pageCount; page++) {
		for (int i = 0; i < m_gridSize; i++) {

			for (int j = 0; j < m_gridSize; j++) {

				m_grid[page][i][j].m_empty = true;
				m_grid[page][i][j].m_image.setTexture(&m_emptyTexure, true);
				m_grid[page][i][j].m_image.setPosition(m_tileSize.x * i, m_tileSize.y * j);
				m_grid[page][i][j].m_image.setSize(m_tileSize);
				std::cout << "page: " << page << " | i: " << i << " | j: " << j << std::endl;
			}
		}
	}
}

void Grid::ResizeGrid(int _newSize, int _gridCount) {
	m_gridCount = _gridCount;
	m_gridSize = _newSize;
	m_tileSize = sf::Vector2f(round(900.0f / m_gridSize), round(900.0f / m_gridSize));
	m_pageCount = round((m_gridCount / (m_gridSize * m_gridSize) + 0.5f));

	// Resize the grid to match the new page count and grid size
	m_grid.resize(m_pageCount, std::vector<std::vector<FileImages>>(m_gridSize, std::vector<FileImages>(m_gridSize)));

	// Update the position and size of each tile in the grid
	for (int page = 0; page < m_pageCount; page++) {
		for (int i = 0; i < m_gridSize; i++) {
			for (int j = 0; j < m_gridSize; j++) {
				// Ensure that the indices i and j are within the bounds of the resized grid
				if (page < m_grid.size() && i < m_grid[page].size() && j < m_grid[page][i].size()) {
					m_grid[page][i][j].m_image.setPosition(m_tileSize.x * i, m_tileSize.y * j);
					m_grid[page][i][j].m_image.setSize(m_tileSize);
					std::cout << "resizze work" << std::endl;
				}
				else {
					// Handle the case where the indices are out of bounds
					// This might indicate a logic error in how you're resizing or accessing the grid
					std::cout << "Error: Attempting to access out-of-bounds grid element." << std::endl;
				}
			}
		}
	}
}

Grid::Grid()
{
	m_pageCount = 1;
	m_gridSize = 10;
	m_gridCount = 9;
	m_tileSize = sf::Vector2f(900/m_gridSize, 900/m_gridSize);
	m_emptyTexure.loadFromFile("emptyTile.png");
}

Grid::~Grid()
{
}
