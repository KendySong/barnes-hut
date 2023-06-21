#include <iostream>

#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>

#include "Sandbox.hpp"
#include "../Math/Math.hpp"
#include "../Simulation/Quad.hpp"
#include "../Simulation/Node.hpp"
#include "../Config.hpp"

Sandbox::Sandbox(sf::RenderWindow* window)
{
	p_window = window;
	m_gravity = 1;
	m_maxForce = 1;

	srand(time(nullptr));
	m_planets.reserve(PLANETS_SPAWN);
	for (size_t i = 0; i < PLANETS_SPAWN; i++)
	{
		m_planets.emplace_back(
			sf::Vector2f(
				Math::random(WIDTH / 2 / 2, WIDTH / 2 + WIDTH / 2 / 2),
				Math::random(HEIGHT / 2 / 2, HEIGHT / 2 + HEIGHT / 2 / 2)
			), 
			5
		);
	}
}

void Sandbox::update(float deltaTime)
{
	//Compute position and size of the global quad
	m_globalRoot.position = m_planets[0].body.getPosition();
	m_globalRoot.size = m_planets[0].body.getSize();
	for (auto& planet : m_planets)
	{
		const sf::Vector2f* position = &planet.body.getPosition();
		const sf::Vector2f* size = &planet.body.getSize();

		m_globalRoot.position.x = m_globalRoot.position.x > position->x ? position->x : m_globalRoot.position.x;
		m_globalRoot.position.y = m_globalRoot.position.y > position->y ? position->y : m_globalRoot.position.y;

		m_globalRoot.size.x = m_globalRoot.size.x < position->x + size->x ? position->x + size->x : m_globalRoot.size.x;
		m_globalRoot.size.y = m_globalRoot.size.y < position->y + size->y ? position->y + size->y : m_globalRoot.size.y;
	}

	m_globalRoot.size.x = m_globalRoot.size.x - m_globalRoot.position.x;
	m_globalRoot.size.y = m_globalRoot.size.y - m_globalRoot.position.y;
	m_globalRoot.construct();
	
	//Generate the quadtree
	m_root = Node(m_globalRoot);
	for (auto& planet : m_planets)
	{
		m_root.insert(&planet);
	}

	//Compute movements
	for (auto& planet : m_planets)
	{
		sf::Vector2f planetPos = planet.body.getPosition();
		for (auto& target : m_planets)
		{
			if (&planet == &target)
			{
				continue;
			}

			sf::Vector2f targetPos = target.body.getPosition();	
			float force = (planet.mass * target.mass) / std::pow(Math::distance(planetPos, targetPos), 2) * m_gravity;
			force = force > m_maxForce ? m_maxForce : force;
			planet.velocity += Math::unit(targetPos - planetPos) * force;
		}

		planet.body.setPosition(planetPos + planet.velocity * deltaTime);
	}
}

void Sandbox::draw() noexcept
{
	for (auto& planet : m_planets)
	{
		p_window->draw(planet.body);
	}
	p_window->draw(m_root.quad.vertices);
	for (auto& node : m_root.quadTree)
	{
		p_window->draw(node->vertices);
	}

	ImGui::Begin("Debug");
		ImGui::DragFloat("Force limit", &m_maxForce);
		ImGui::DragFloat("Gravity", &m_gravity);
	ImGui::End();
}