#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

#include "Camera.hpp"

#include "../Simulation/Planet.hpp"
#include "../Simulation/Quad.hpp"
#include "../Simulation/Node.hpp"

class Sandbox
{
public :
	Sandbox(sf::RenderWindow* window);
	void update(float deltaTime);
	void draw() noexcept;

private :
	sf::RenderWindow* p_window;
	Camera m_camera;
	std::vector<Planet> m_planets;
	float m_gravity;
	float m_maxForce;

	bool m_useBarneHut;
	Quad m_globalRoot;
	Node m_root;
};