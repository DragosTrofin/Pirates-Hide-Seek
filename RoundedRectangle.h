#pragma once
#include <SFML/Graphics/Shape.hpp>


class RoundedRectangle : public sf::Shape
{
public:

    explicit RoundedRectangle(const sf::Vector2f& size = sf::Vector2f(0, 0), float radius = 0, std::size_t cornerPointCount = 10);
    void setSize(const sf::Vector2f& size);
    void setCornerRadius(float radius);
    void setPointCount(std::size_t count);
    const sf::Vector2f& getSize() const;
    float getCornerRadius() const;
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;

private:

    sf::Vector2f size;
    float radius;
    std::size_t cornerPointCount;
};