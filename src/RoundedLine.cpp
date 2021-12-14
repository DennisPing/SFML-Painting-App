#define _USE_MATH_DEFINES

#include "RoundedLine.hpp"
#include <cmath>

RoundedLine::RoundedLine(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, const float width, sf::Color color, const short& port)
        : m_startPoint (startPoint), m_endPoint (endPoint), m_Width (width), m_color (color), m_owner (port) {
    setPosition(m_startPoint);
    setFillColor(m_color);
    update();
}

void RoundedLine::setEndPoint(const sf::Vector2f& endPoint) {
    m_endPoint = endPoint;
    update();
}

void RoundedLine::setWidth(const float width) {
    m_Width = width;
    update();
}

size_t RoundedLine::getPointCount() const {
    return 30;
}

sf::Vector2f RoundedLine::getPoint(size_t index) const {
    sf::Vector2f P1(1.0, 0.0);
    sf::Vector2f P2(m_endPoint + sf::Vector2f(1.0, 0.0) - getPosition());

    sf::Vector2f offset;
    int iFlipDirection;

    if (index < 15) {
        offset = P2;
        iFlipDirection = 1;
    }
    else {
        offset = P1;
        iFlipDirection = -1;
        index -= 15;
    }

    float start = -atan2(P1.y - P2.y, P2.x - P1.x);

    float angle = index * M_PI / 14 - M_PI / 2 + start;
    float x = std::cos(angle) * m_Width / 2;
    float y = std::sin(angle) * m_Width / 2;

    return sf::Vector2f(offset.x + x * iFlipDirection, offset.y + y * iFlipDirection);
}

bool RoundedLine::execute(sf::RenderTexture& render_texture) {
    render_texture.draw(*this);
    return true;
}

bool RoundedLine::undo(sf::RenderTexture& render_texture) {
    this->setFillColor(sf::Color::White);
    update();
    render_texture.draw(*this);
    return true;
}

bool RoundedLine::redo(sf::RenderTexture& render_texture) {
    this->setFillColor(m_color);
    update();
    render_texture.draw(*this);
    return true;
}

short RoundedLine::getOwner() const {
    return m_owner;
}

std::string RoundedLine::description() {
    return "(" + std::to_string((int)m_startPoint.x) + "," + std::to_string((int)m_startPoint.y) + ") to (" + std::to_string((int)m_endPoint.x) + "," + std::to_string((int)m_endPoint.y) + ")";
}