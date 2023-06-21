#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

#include "../Simulation/Planet.hpp"
#include "../Simulation/Quad.hpp"

class Sandbox
{
public :
	Sandbox(sf::RenderWindow* window);
	void update(float deltaTime);
	void draw() noexcept;

private :
	sf::RenderWindow* p_window;
	std::vector<Planet> m_planets;
	float m_gravity;
	float m_maxForce;

	Quad m_globalRoot;
};