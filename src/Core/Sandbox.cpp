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
	/*
	Galaxy galaxy(midScreen - sf::Vector2f(WIDTH/8, HEIGHT/8), 100, PLANETS_SPAWN, m_planets);
	Galaxy galaxy2(midScreen + sf::Vector2f(WIDTH/8, HEIGHT/8), 100, PLANETS_SPAWN, m_planets);

	for (size_t i = 0; i < PLANETS_SPAWN; i++)
	{
		m_planets.emplace_back(
			Math::randomCircle(200) += midScreen,
			1, 5
		);
	}
	*/

	p_window = window;
	m_gravity = GRAVITY;
	m_maxForce = MAX_FORCE;
	m_useBarneHut = true;
	sf::Vector2f midScreen = sf::Vector2f(WIDTH / 2, HEIGHT / 2);

	m_fps = 0;
	m_camera = Camera(2000, midScreen);

	srand(time(nullptr));
	m_planets.reserve(PLANETS_SPAWN);
	//Galaxy galaxy(midScreen, 100, PLANETS_SPAWN, m_planets);	
	for (size_t i = 0; i < PLANETS_SPAWN; i++)
	{
		m_planets.emplace_back(
			Math::randomCircle(200) += midScreen,
			3
		);
	}
	m_nbPlanets = "Planets : " + std::to_string(m_planets.size());
}

void Sandbox::update(float deltaTime)
{
	m_fps++;
	if (m_fpsInterval.getElapsedTime().asSeconds() >= 1)
	{
		m_fpsText = "FPS : " + std::to_string(m_fps);
		m_fpsInterval.restart();
		m_fps = 0;
	}

	m_camera.manageMovements(deltaTime);

	if (m_useBarneHut)
	{
		VertexQuadTree::instance()->quads.clear();
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

		//Generate the quadtree
		m_root = Node(m_globalRoot);
		for (auto& planet : m_planets)
		{
			m_root.insert(&planet);
		}
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
		auto& vertices = VertexQuadTree::instance()->quads;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			p_window->draw(vertices[i]);
		}
	}

	for (auto& planet : m_planets)
	{
		p_window->draw(planet.body);
	}

	ImGui::Begin("Debug");
		ImGui::TextUnformatted(m_fpsText.c_str());
		ImGui::TextUnformatted(m_nbPlanets.c_str());
		ImGui::Checkbox("Use BarnesHut", &m_useBarneHut);
		ImGui::DragFloat("Force limit", &m_maxForce, 0.01f);
		ImGui::DragFloat("Gravity", &m_gravity, 0.1f);
	ImGui::End();
}