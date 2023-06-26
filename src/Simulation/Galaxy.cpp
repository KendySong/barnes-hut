#define _USE_MATH_DEFINES
#include <cmath>

#include "Galaxy.hpp"
#include "../Math/Math.hpp"

Galaxy::Galaxy(sf::Vector2f position, float size, std::uint32_t nbPlanets, std::vector<Planet>& planets)
{
	planets.reserve(nbPlanets);
	for (size_t i = 0; i < nbPlanets; i++)
	{
		float length = Math::random(0, size);
		float angle = Math::random(0, 2 * M_PI);
		Planet planet(sf::Vector2f(cos(angle) * length, sin(angle) * length) + position, 10);

		angle -= M_PI_4;
		sf::Vector2f targetDir = sf::Vector2f(cos(angle) * length, sin(angle) * length) + position;
		planet.velocity = Math::unit(targetDir - position) * 1500;

		planets.push_back(planet);
	}
}