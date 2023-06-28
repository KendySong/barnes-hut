#include "Config.hpp"

const std::uint32_t Config::Width = 1280;
const std::uint32_t Config::Height = 720;
const std::uint32_t Config::PlanetSpwan = 150;

float Config::gravity = 500.0f;
float Config::maxForce = 0.01f;
const sf::Vector2f Config::BorderOffset = sf::Vector2f(10, 10);
const sf::Vector2f Config::BorderOffsetSize = Config::BorderOffset * 2;