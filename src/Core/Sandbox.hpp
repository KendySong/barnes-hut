#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

#include "Camera.hpp"

#include "../Simulation/Planet.hpp"
#include "../Simulation/Quad.hpp"
#include "../Simulation/Node.hpp"
#include "../Simulation/Galaxy.hpp"

class Sandbox
{
public :
	Sandbox(sf::RenderWindow* window);
	void update(float deltaTime);
	void draw() noexcept;

private :
	Camera m_camera;
	sf::RenderWindow* p_window;
	
	std::string m_nbPlanets;
	std::uint32_t m_fps;
	std::string m_fpsText;
	sf::Clock m_fpsInterval;

	std::vector<Planet> m_planets;
	float m_gravity;
	float m_maxForce;

	bool m_useBarneHut;
	Quad m_globalRoot;
	Node m_root;
};