#include <iostream>

#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>

#include "Sandbox.hpp"
#include "../Math/Math.hpp"
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
		m_planets.emplace_back(sf::Vector2f(rand() % WIDTH, rand() & HEIGHT), 20);
	}
}

void Sandbox::update(float deltaTime)
{
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

	ImGui::Begin("Debug");
		
		ImGui::DragFloat("Force limit", &m_maxForce);
		ImGui::DragFloat("Gravity", &m_gravity);
	ImGui::End();
}