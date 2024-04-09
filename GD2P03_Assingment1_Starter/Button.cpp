#include "Button.h"
void screenshots(const std::string& _fileSaveLocation, sf::Window* _window) {
    sf::Texture texture;
    texture.create(_window->getSize().x, _window->getSize().y);
    texture.update(*_window);
    if (texture.copyToImage().saveToFile(_fileSaveLocation)) {
        std::cout << "screenshot saved to " << _fileSaveLocation << std::endl;
    }
}
bool Button::update(const sf::Vector2f& _mousePos, int& _pageRef, Grid& _gridRef, bool* _imageList)
{

    if (m_button.getGlobalBounds().contains(_mousePos)) {
        m_button.setFillColor(m_hoverColor);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            if (!click) {
                switch (m_type)
                {
                case PAGE_LEFT:
                    if(_pageRef>0){
                        _pageRef--;
                    }
                    break;
                case PAGE_RIGHT:
                    if (_pageRef < _gridRef.m_pageCount-1) {
                        _pageRef++;
                    }
                    break;
                case ZOOM_IN:
                    if(_gridRef.m_gridSize > 0 ){
                        _gridRef.ResizeGrid(-1);
                    }
                    break;
                case ZOOM_OUT:
                    _gridRef.ResizeGrid(1);
                    break;
                case DONE:
                    return true;
                    break;
                case FILE_SIZE:
                    *_imageList = !*_imageList;
                    break;   
                case SCREENSHOT:
                    screenshots("savedMosaic.png", m_window);
                    std::cout << "Image Saved" << std::endl;
                    break;
                default:
                    break;
                }
            }
            click = true;
            m_button.setFillColor(m_activeColor);
        }
    }
    else{
        m_button.setFillColor(m_idleColor);
    }
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        click = false;
    }
    return false;
}

void Button::draw(sf::RenderWindow& _window)
{
    _window.draw(m_button);
    _window.draw(m_text);
}

Button::Button(const sf::Vector2f& _position, const sf::Vector2f& _size, const sf::Font& _font,
    const std::string& _text, sf::Color _idleColor, sf::Color _hoverColor, sf::Color _activeColor, buttonType _type)
{
    m_type = _type; 
	m_idleColor = _idleColor;
	m_activeColor = _activeColor;
	m_hoverColor = _hoverColor;
	m_button = sf::RectangleShape(_size);
	m_text = sf::Text(_text, _font, 24);
    
    m_button.setPosition(_position);
    m_text.setPosition(_position);
    m_text.setFillColor(sf::Color::Black);

}

Button::~Button()
{
}
