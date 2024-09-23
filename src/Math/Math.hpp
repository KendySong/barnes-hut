#pragma once
#include <SFML/System.hpp>

#include "../Simulation/Planet.hpp"

class Math
{
public :
	static float length(sf::Vector2f v) noexcept;
	static float distance(sf::Vector2f a, sf::Vector2f b) noexcept;
	static float approximateDistance(sf::Vector2f a, sf::Vector2f b) noexcept;
	static sf::Vector2f unit(sf::Vector2f v) noexcept;
	static float random(float min, float max) noexcept;

	static sf::Vector2f randomCircle(float radius) noexcept;
	static float interpolate(float a, float b, float t) noexcept;

	static sf::Vector2f force(Planet* a, Planet* b) noexcept;
	static sf::Vector2f force(Planet* a, const sf::Vector2f& position, float mass) noexcept;
};

sf::Vector2f operator*(sf::Vector2f v, float t);