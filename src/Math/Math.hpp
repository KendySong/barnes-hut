#pragma once
#include <SFML/System.hpp>

class Math
{
public :
	static float length(sf::Vector2f v) noexcept;
	static float distance(sf::Vector2f a, sf::Vector2f b) noexcept;
	static sf::Vector2f unit(sf::Vector2f v) noexcept;
	static float random(float min, float max) noexcept;

	static sf::Vector2f randomCircle(float radius) noexcept;
};