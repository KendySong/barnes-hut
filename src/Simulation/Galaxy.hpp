#pragma once
#include <vector>

#include <SFML/System.hpp>

#include "Planet.hpp"

class Galaxy
{
public :
	Galaxy(sf::Vector2f position, float size, std::uint32_t nbPlanets, std::vector<Planet>& planets);

private :

};