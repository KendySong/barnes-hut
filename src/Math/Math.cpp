#include <cmath>

#include "Math.hpp"

float Math::length(sf::Vector2f v) noexcept
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

float Math::distance(sf::Vector2f a, sf::Vector2f b) noexcept
{
	return Math::length(b - a);
}

sf::Vector2f Math::unit(sf::Vector2f v) noexcept
{
	return v / Math::length(v);
}

float Math::random(float min, float max) noexcept
{
	float random = (float)rand() / (float)RAND_MAX;
	float delta = max - min;
	float r = random * delta;
	return min + r;
}