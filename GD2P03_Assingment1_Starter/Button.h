#pragma once
#include <SFML/Graphics.hpp>
class Button
{


    private:
        sf::RectangleShape m_button;
        sf::Text m_text;
        sf::Color m_idleColor;
        sf::Color m_hoverColor;
        sf::Color m_activeColor;
    public:
        void update(const sf::Vector2f& _mousePos);
        void draw(sf::RenderWindow& _window);
        Button(const sf::Vector2f& _position, const sf::Vector2f& _size, const sf::Font& _font,
            const std::string& _text, sf::Color _idleColor, sf::Color _hoverColor, sf::Color _activeColor);
        ~Button();
};

