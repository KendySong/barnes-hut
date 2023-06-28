#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>

#include "Math/Math.hpp"

class Config
{
public :
	static const std::uint32_t Width;
	static const std::uint32_t Height;
	static const std::uint32_t PlanetSpwan;
	
	static const sf::Vector2f BorderOffset;
	static const sf::Vector2f BorderOffsetSize;

	static float gravity;
	static float maxForce;
};