#include "Grid.h"

void Grid::setTileTexture(int _x, int _y, sf::Texture _texture)
{
    m_grid[_x][_y].m_image.setTexture(&_texture,false);
    return;
}

void Grid::addTile(FileImages tile)
{
    int x = 0;
    int y = 0;


    bool notDone = true;
        while(notDone){
            for (int i = 0; i < m_grid.size(); i++) {
                std::cout << "GRID SIZE: " << m_grid.size() << std::endl;
                for (int j = 0; j < m_grid[i].size(); j++) {
                    if (m_grid[i][j].m_empty) {
                        tile.m_image.setTexture(&txt);
                        m_grid[i][j]= tile;
                        notDone = false;
                        std::cout << m_grid.size() << " | " << m_grid[0].size() << std::endl;
                        return;
                    }
                }
            }
            addRowCol(); 
            std::cout << "new rowcol" << std::endl;
        }


        return;
}
void Grid::addRowCol() {

    int numColumns = m_grid.empty() ? 0 : m_grid[0].size();

    std::vector<FileImages> newRow(numColumns, FileImages(""));
    
    m_grid.push_back(newRow);

    FileImages emptyFile("");
    emptyFile.m_image.setTexture(&txt);
    for (auto& row : m_grid) {
        row.push_back(emptyFile);
    }

   
    return;
}

void Grid::SetupImages(float _size)
{
    std::cout << m_grid.size() << " - "  << m_grid[0].size() << std::endl;
    
    for (size_t i = 0; i < m_grid.size(); i++) {
        for (size_t j = 0; j < m_grid[i].size(); j++) {
            m_grid[i][j].m_image.setSize(sf::Vector2f(_size, _size));
            m_grid[i][j].m_image.setPosition(sf::Vector2f((float)i * _size, (float)j * _size));
            std::cout << i << " - " << j << std::endl;
            if (m_grid[i][j].m_empty) {
                std::cout << "empty" << std::endl;
                
                m_grid[i][j].m_image.setTexture(&txt);
            }
            
        }
    }
}


Grid::Grid()
{
    txt.loadFromFile("Images/empty.png");
}

Grid::~Grid()
{
}
