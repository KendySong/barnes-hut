#include "Planet.hpp"

#include "../Math/Math.hpp"

Planet::Planet(sf::Vector2f position, float mass)
{
	body = sf::VertexArray(sf::Points, 1);
	body[0].color = sf::Color::Red;
	body[0].position = position;
	this->velocity = sf::Vector2f(
		Math::random(-1, 1), 
		0
	);

	this->mass = mass;
}