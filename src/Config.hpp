#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>

const std::uint32_t WIDTH = 1280;
const std::uint32_t HEIGHT = 720;

const std::uint32_t PLANETS_SPAWN = 150;
const float GRAVITY = 500;
const float MAX_FORCE = 0.01;

const sf::Vector2f BORDER_OFFSET = sf::Vector2f(10, 10);
const sf::Vector2f BORDER_OFFSET_SIZE = BORDER_OFFSET + BORDER_OFFSET;