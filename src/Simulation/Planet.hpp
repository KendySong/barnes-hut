#pragma once
#include <SFML/Graphics.hpp>

struct Planet
{
public :
	Planet(sf::Vector2f position, float mass);

	float mass;
	sf::Vector2f velocity;
	sf::VertexArray body;
};