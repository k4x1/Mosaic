#include "Button.h"

// Function to take a screenshot of the window and save it to a file
void screenshots(const std::string& _fileSaveLocation, sf::Window* _window) {
    sf::Texture texture;
    // Create a texture with the size of the window
    texture.create(_window->getSize().x, _window->getSize().y);
    // Update the texture with the current window content
    texture.update(*_window);
    // Save the texture as an image file
    if (texture.copyToImage().saveToFile(_fileSaveLocation)) {
        std::cout << "screenshot saved to " << _fileSaveLocation << std::endl;
    }
}

// Method to update the button's state based on mouse position and clicks
bool Button::update(const sf::Vector2f& _mousePos, std::atomic<int>& _pageRef, Grid& _gridRef, bool* _imageList) {
    // Check if the mouse is over the button
    if (m_button.getGlobalBounds().contains(_mousePos)) {
        // Change the button color to hover color
        m_button.setFillColor(m_hoverColor);
        // Check if the left mouse button is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            // If the button has not been clicked yet
            if (!click) {
                // Perform action based on button type
                switch (m_type) {
                case PAGE_LEFT:
                    // Decrement page reference if it's greater than 0
                    if (_pageRef > 0) {
                        _pageRef.fetch_add(-1);
                    }
                    break;
                case PAGE_RIGHT:
                    // Increment page reference if it's less than the total page count
                    if (_pageRef < _gridRef.m_pageCount - 1) {
                        _pageRef.fetch_add(1);
                    }
                    break;
                case ZOOM_IN:
                    // Decrease grid size if it's greater than 0
                    if (_gridRef.m_gridSize > 0) {
                        _gridRef.ResizeGrid(-1);
                    }
                    break;
                case ZOOM_OUT:
                    // Increase grid size
                    _gridRef.ResizeGrid(1);
                    break;
                case DONE:
                    // Return true to indicate the button has been clicked
                    return true;
                    break;
                case FILE_SIZE:
                    // Toggle the image list boolean
                    *_imageList = !*_imageList;
                    break;
                case SCREENSHOT:
                    // Take a screenshot and save it
                    screenshots("savedMosaic.png", m_window);
                    std::cout << "Image Saved" << std::endl;
                    break;
                default:
                    break;
                }
                // Set the button as clicked
                click = true;
                // Change the button color to active color
                m_button.setFillColor(m_activeColor);
            }
        }
    }
    else {
        // If the mouse is not over the button, set the button color to idle color
        m_button.setFillColor(m_idleColor);
    }
    // If the left mouse button is not pressed, set the button as not clicked
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        click = false;
    }
    // Return false to indicate the button has not been clicked
    return false;
}

// Method to draw the button and its text on the window
void Button::draw(sf::RenderWindow& _window) {
    _window.draw(m_button);
    _window.draw(m_text);
}

Button::Button(const sf::Vector2f& _position, const sf::Vector2f& _size, const sf::Font& _font,
    const std::string& _text, sf::Color _idleColor, sf::Color _hoverColor, sf::Color _activeColor, buttonType _type) {
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


Button::~Button() {
}
