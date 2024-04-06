#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.h"
enum buttonType {
    PAGE_LEFT,
    PAGE_RIGHT,
    ZOOM_IN,
    ZOOM_OUT
};
class Button
{


    private:
        bool click = false;
        sf::RectangleShape m_button;
        sf::Text m_text;
        sf::Color m_idleColor;
        sf::Color m_hoverColor;
        sf::Color m_activeColor;
        buttonType m_type;
    public:


        void update(const sf::Vector2f& _mousePos, int& _pageRef, Grid& _gridRef);
        void draw(sf::RenderWindow& _window);
        Button(const sf::Vector2f& _position, const sf::Vector2f& _size, const sf::Font& _font,
            const std::string& _text, sf::Color _idleColor, sf::Color _hoverColor, sf::Color _activeColor, buttonType _type);
        ~Button();
};

