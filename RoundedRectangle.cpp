#include "RoundedRectangle.h"
#include <cmath>

RoundedRectangle::RoundedRectangle(const sf::Vector2f& size, float radius, std::size_t cornerPointCount)
{
    this->size = size;
    this->radius = radius;
    this->cornerPointCount = cornerPointCount;
    update();
}

void RoundedRectangle::setSize(const sf::Vector2f& size)
{
    this->size = size;
    update();
}

void RoundedRectangle::setCornerRadius(float radius)
{
    this->radius = radius;
    update();
}

void RoundedRectangle::setPointCount(std::size_t count)
{
    this->cornerPointCount = count;
    update();
}

const sf::Vector2f& RoundedRectangle::getSize() const
{
    return size;
}

float RoundedRectangle::getCornerRadius() const
{
    return radius;
}

std::size_t RoundedRectangle::getPointCount() const
{
    return cornerPointCount * 4;
}

sf::Vector2f RoundedRectangle::getPoint(std::size_t index) const
{
    if (index >= cornerPointCount * 4)
    {
        return sf::Vector2f(0, 0);
    }

    float deltaAngle{ 90.0f / (cornerPointCount - 1) };
    std::size_t centerIndex{ index / cornerPointCount };
    static const float pi{ 3.141592654f };
    
    sf::Vector2f center;
    switch (centerIndex)
    {
        case 0: 
        {
            center.x = size.x - radius; 
            center.y = radius; 
            break;
        }
        case 1: 
        {
            center.x = radius; 
            center.y = radius; 
            break;
        }
        case 2: 
        {
            center.x = radius; 
            center.y = size.y - radius; 
            break;
        }
        case 3:
        {
            center.x = size.x - radius; 
            center.y = size.y - radius; 
            break;
        }
    }

    float angle{ deltaAngle * (index - centerIndex) * pi / 180 };
    return sf::Vector2f(radius * cos(angle) + center.x, -radius * sin(angle) + center.y);
}
