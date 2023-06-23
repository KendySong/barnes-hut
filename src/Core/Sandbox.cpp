#include <iostream>

#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>

#include "Sandbox.hpp"
#include "../Math/Math.hpp"
#include "../Simulation/Quad.hpp"
#include "../Simulation/Node.hpp"
#include "../Simulation/VertexQuadTree.hpp"
#include "../Config.hpp"

Sandbox::Sandbox(sf::RenderWindow* window)
{
	p_window = window;
	m_gravity = GRAVITY;
	m_maxForce = MAX_FORCE;
	m_useBarneHut = true;

	m_camera = Camera(500, sf::Vector2f(WIDTH / 2, HEIGHT / 2));

	srand(time(nullptr));
	m_planets.reserve(PLANETS_SPAWN);
	for (size_t i = 0; i < PLANETS_SPAWN; i++)
	{
		m_planets.emplace_back(
			Math::randomCircle(100) += sf::Vector2f(WIDTH / 2, HEIGHT / 2),
			3
		);
	}
}

void Sandbox::update(float deltaTime)
{
	m_camera.manageMovements(deltaTime);

	if (m_useBarneHut)
	{
		m_globalRoot.position = m_planets[0].body[0].position;
		m_globalRoot.size = sf::Vector2f(1, 1);

		//Construct the quad that contains all particules by getting the limits
		for (auto& planet : m_planets)
		{
			const sf::Vector2f& position = planet.body[0].position;

			m_globalRoot.position.x = m_globalRoot.position.x > position.x ? position.x : m_globalRoot.position.x;
			m_globalRoot.position.y = m_globalRoot.position.y > position.y ? position.y : m_globalRoot.position.y;

			m_globalRoot.size.x = m_globalRoot.size.x < position.x ? position.x : m_globalRoot.size.x;
			m_globalRoot.size.y = m_globalRoot.size.y < position.y ? position.y : m_globalRoot.size.y;	
		}

		//Compute the size of the box and add border
		m_globalRoot.size = m_globalRoot.size - m_globalRoot.position;
		m_globalRoot.position -= BORDER_OFFSET;
		m_globalRoot.size += BORDER_OFFSET_SIZE;

		//Set the quad to a square
		m_globalRoot.size.x = m_globalRoot.size.x < m_globalRoot.size.y ? m_globalRoot.size.y : m_globalRoot.size.x;
		m_globalRoot.size.y = m_globalRoot.size.y < m_globalRoot.size.x ? m_globalRoot.size.x : m_globalRoot.size.x;
		m_globalRoot.construct();
	}
	else
	{
		for (auto& planet : m_planets)
		{
			sf::Vector2f planetPos = planet.body[0].position;
			for (auto& target : m_planets)
			{
				if (&planet == &target)
				{
					continue;
				}

				sf::Vector2f targetPos = target.body[0].position;
				float force = (planet.mass * target.mass) / std::pow(Math::distance(planetPos, targetPos), 2) * m_gravity;
				force = force > m_maxForce ? m_maxForce : force;
				planet.velocity += Math::unit(targetPos - planetPos) * force;
			}

			planet.body[0].position = planetPos + planet.velocity / planet.mass * deltaTime;
		}
	}	
}

void Sandbox::draw() noexcept
{
	p_window->setView(m_camera.getView());
	
	if (m_useBarneHut)
	{
		p_window->draw(m_globalRoot.vertices);
	}

	for (auto& planet : m_planets)
	{
		p_window->draw(planet.body);
	}

	ImGui::Begin("Debug");
		ImGui::Checkbox("Use BarnesHut", &m_useBarneHut);
		ImGui::DragFloat("Force limit", &m_maxForce);
		ImGui::DragFloat("Gravity", &m_gravity);
	ImGui::End();
}