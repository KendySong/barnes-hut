#define _USE_MATH_DEFINES
#include <cmath>

#include "Math.hpp"
#include "../Config.hpp"

float Math::length(sf::Vector2f v) noexcept
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

float Math::distance(sf::Vector2f a, sf::Vector2f b) noexcept
{
	return Math::length(b - a);
}

float Math::manhattanDistance(sf::Vector2f a, sf::Vector2f b) noexcept
{
	auto&v = b - a;
	return abs(v.x) + abs(v.y);
}

sf::Vector2f Math::unit(sf::Vector2f v) noexcept
{
	return v / Math::length(v);
}

float Math::random(float min, float max) noexcept
{
	float random = (float)rand() / (float)RAND_MAX;
	float delta = max - min;
	float r = random * delta;
	return min + r;
}

sf::Vector2f Math::randomCircle(float radius) noexcept
{
	float theta = Math::random(0, 2 * M_PI);
	float length = Math::random(0, radius);
	return { cos(theta) * length, sin(theta) * length };
}

float Math::interpolate(float a, float b, float t) noexcept
{
	return (b - a) * t + a;
}

sf::Vector2f operator*(sf::Vector2f v, float t)
{
	return { v.x * t, v.y * t };
}

sf::Vector2f Math::force(Planet* a, Planet* b) noexcept
{
	const sf::Vector2f& targetPos = b->body[0].position;
	const sf::Vector2f& planetPos = a->body[0].position;

	float force = (a->mass * b->mass) / std::pow(Math::distance(planetPos, targetPos), 2) * Config::gravity;
	force = force > Config::maxForce ? Config::maxForce : force;
	return Math::unit(targetPos - planetPos) * force;
}

sf::Vector2f Math::force(Planet* a, const sf::Vector2f& position, float mass) noexcept
{
	const sf::Vector2f& planetPos = a->body[0].position;

	float force = (a->mass * mass) / std::pow(Math::distance(planetPos, position), 2) * Config::gravity;
	force = force > Config::maxForce ? Config::maxForce : force;
	return Math::unit(position - planetPos) * force;
}