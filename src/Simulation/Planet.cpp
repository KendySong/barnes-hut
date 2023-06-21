#include "Planet.hpp"

#include "../Math/Math.hpp"

Planet::Planet(sf::Vector2f position, float mass)
{
	body = sf::RectangleShape(sf::Vector2f(5, 5));
	body.setPosition(position);
	this->velocity = sf::Vector2f(
		Math::random(-1, 1), 
		0
	);

	this->mass = mass;
}