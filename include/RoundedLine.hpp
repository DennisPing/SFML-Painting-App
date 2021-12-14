#ifndef ROUNDEDLINE_HPP
#define ROUNDEDLINE_HPP

// Class written by Foaly

#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <string>


class RoundedLine : public sf::Shape
{
public:

    RoundedLine(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, const float width, const sf::Color color, const short& port);

    void setEndPoint(const sf::Vector2f& endPoint);

    void setWidth(const float width);

    virtual size_t getPointCount() const;

    virtual sf::Vector2f getPoint(size_t index) const;

    virtual bool execute(sf::RenderTexture& render_texture);

    virtual bool undo(sf::RenderTexture& render_texture);

    virtual bool redo(sf::RenderTexture& render_texture);

    virtual short getOwner() const;

    virtual std::string description();

private :
    sf::Vector2f m_startPoint;
    sf::Vector2f m_endPoint;
    float m_Width;
    sf::Color m_color;
    short m_owner;
};
#endif // ROUNDEDLINE_HPP