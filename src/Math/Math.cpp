#define _USE_MATH_DEFINES
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

sf::Vector2f Math::randomCircle(float radius) noexcept
{
	float theta = Math::random(0, 2 * M_PI);
	float length = Math::random(0, radius);
	return { cos(theta) * length, sin(theta) * length };
}

float Math::interpolate(float a, float b, float t) noexcept
{
	return (b - a) * t + a;
}