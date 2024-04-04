#include "Button.h"

void Button::update(const sf::Vector2f& _mousePos)
{
    if (m_button.getGlobalBounds().contains(_mousePos)) {
        m_button.setFillColor(m_hoverColor);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            m_button.setFillColor(m_activeColor);
        }
    }
    else {
        m_button.setFillColor(m_idleColor);
    }
}

void Button::draw(sf::RenderWindow& _window)
{
    _window.draw(m_button);
    _window.draw(m_text);
}

Button::Button(const sf::Vector2f& _position, const sf::Vector2f& _size, const sf::Font& _font, const std::string& _text, sf::Color _idleColor, sf::Color _hoverColor, sf::Color _activeColor)
{
	m_idleColor = _idleColor;
	m_activeColor = _activeColor;
	m_hoverColor = _hoverColor;
	m_button = sf::RectangleShape(_size);
	m_text = sf::Text(_text, _font, 24);

}

Button::~Button()
{
}
