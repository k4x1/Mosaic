#include "Grid.h"

// Method to set the texture for a tile in the grid
void Grid::setTileTextures(sf::Texture* _texture)
{
  
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

// Method to initialize the grid with a specified number of tiles
void Grid::InitGrid(int _gridCount)
{

    m_gridCount = _gridCount;
    m_pageCount = round((m_gridCount / (m_gridSize * m_gridSize) + 0.5f));
    m_grid.resize(m_pageCount, std::vector<std::vector<FileImages>>(m_gridSize, std::vector<FileImages>(m_gridSize)));

    for (int page = 0; page < m_pageCount; page++) {
        for (int i = 0; i < m_gridSize; i++) {
            for (int j = 0; j < m_gridSize; j++) {
                m_grid[page][i][j].m_empty = true;
                m_grid[page][i][j].m_image = m_emptyShape;
                m_grid[page][i][j].m_image.setPosition(m_tileSize.x * i, m_tileSize.y * j);
                m_grid[page][i][j].m_image.setSize(m_tileSize);
            }
        }
    }
}
//
// Method to resize the grid by a specified amount
void Grid::ResizeGrid(int _newSize) {
    m_gridSize += _newSize;
    m_tileSize = sf::Vector2f(round(900.0f / m_gridSize), round(900.0f / m_gridSize));
}

void Grid::AnimateGrid()
{

    if (m_clock.getElapsedTime().asSeconds() > 0.5f) {
      
        if (m_rectSourceSprite.left == 2160) {
            m_rectSourceSprite.left = 0;
        }
        else {
           m_rectSourceSprite.left += 1080;
        }
        for (int page = 0; page < m_pageCount; page++) {
            for (int i = 0; i < m_gridSize; i++) {
                for (int j = 0; j < m_gridSize; j++) {
                    if (m_grid[page][i][j].m_empty) {
                        m_grid[page][i][j].m_image.setTextureRect(m_rectSourceSprite);
                    }
                }
            }
        }
   
         m_clock.restart();
        
    }
}

// Constructor for the Grid class
Grid::Grid()
{
    m_emptyShape.setTexture(&m_emptyTexure, true);
    m_emptyShape.setTextureRect(m_rectSourceSprite);
    m_pageCount = 1;
    m_gridSize = 2;
    m_gridCount = 9;
    m_tileSize = sf::Vector2f(900 / m_gridSize, 900 / m_gridSize);
    m_emptyTexure.loadFromFile("loadingTileSheet.png");
}

// Destructor for the Grid class
Grid::~Grid()
{
}
