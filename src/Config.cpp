#include "Config.hpp"

const std::uint32_t Config::Width = 1280;
const std::uint32_t Config::Height = 720;
const std::uint32_t Config::PlanetSpwan = 500;

const sf::Vector2f Config::BorderOffset = sf::Vector2f(10, 10);
const sf::Vector2f Config::BorderOffsetSize = Config::BorderOffset * 2;

float Config::gravity = 500;
float Config::maxForce = 0.01; 
float Config::thresholdCompute = 0.1f;
bool Config::displayQuadtree = true;