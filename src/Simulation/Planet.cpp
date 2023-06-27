#include "Planet.hpp"

#include "../Math/Math.hpp"

Planet::Planet(sf::Vector2f position, float mass)
{
	body = sf::VertexArray(sf::Points, 1);
	body[0].color = sf::Color::Red;
	body[0].position = position;
	this->velocity = sf::Vector2f(
		Math::random(-50, 50), 
		0
	);

	this->mass = mass;
}

Planet::Planet(sf::Vector2f position, float minMass, float maxMass)
{
	static auto a = sf::Vector3f(255, 255, 255);
	static auto b = sf::Vector3f(0, 128, 128);

	this->mass = Math::random(minMass, maxMass);
	body = sf::VertexArray(sf::Points, 1);	
	body[0].position = position; 
	this->velocity = sf::Vector2f(
		Math::random(-50, 50),
		0
	);

	float t = (this->mass - minMass) / (maxMass - minMass);
	sf::Vector3f color = t * (a - b) + b;
	body[0].color = sf::Color(color.x, color.y, color.z);
}