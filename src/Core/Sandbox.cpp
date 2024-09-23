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
	Galaxy galaxy(midScreen - sf::Vector2f(Config::Width / 8, Config::Height / 8), 100, Config::PlanetSpwan, m_planets);
	Galaxy galaxy2(midScreen + sf::Vector2f(Config::Width / 8, Config::Height / 8), 100, Config::PlanetSpwan, m_planets);
	for (size_t i = 0; i < PLANETS_SPAWN; i++)
	{
		m_planets.emplace_back(
			Math::randomCircle(200) += midScreen,
			1, 5
		);
	}
	*/

	p_window = window;
	m_pause = true;
	m_useBarneHut = true;
	sf::Vector2f midScreen = sf::Vector2f(Config::Width / 2, Config::Height / 2);

	m_fps = 0;
	m_camera = Camera(2000, midScreen);

	srand(time(nullptr));
	m_planets.reserve(Config::PlanetSpwan);
	Galaxy galaxy(midScreen, 100, Config::PlanetSpwan, m_planets);
	m_nbPlanets = "Planets : " + std::to_string(m_planets.size());
	_pixels = sf::VertexArray(sf::PrimitiveType::Points, Config::PlanetSpwan);
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

	if (m_pause)
	{
		return;
	}

	if (m_useBarneHut)
	{
		VertexQuadTree::instance()->quads.clear();
		m_globalRoot.position = m_planets[0].body[0].position;
		m_globalRoot.size = sf::Vector2f(1, 1);

		//Construct the quad that contains all particules by getting
		//the position of the planets at the extremity of the group
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
		m_globalRoot.position -= Config::BorderOffset;
		m_globalRoot.size += Config::BorderOffsetSize;

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

		for (auto& planet : m_planets)
		{
			m_root.computeForce(&planet);
			planet.body[0].position = planet.body[0].position + planet.velocity / planet.mass * deltaTime;
		}
	}
	else
	{
		for (auto& planet : m_planets)
		{
			for (auto& target : m_planets)
			{
				if (&planet == &target)
				{
					continue;
				}
				planet.velocity += Math::force(&planet, &target);
			}

			planet.body[0].position = planet.body[0].position + planet.velocity / planet.mass * deltaTime;
		}
	}	
}

void Sandbox::draw() noexcept
{
	p_window->setView(m_camera.getView());
	if (Config::displayQuadtree && m_useBarneHut)
	{
		p_window->draw(m_globalRoot.vertices);
		auto& vertices = VertexQuadTree::instance()->quads;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			p_window->draw(vertices[i]);
		}
	}

	for (size_t i = 0; i < m_planets.size(); i++)
	{
		_pixels[i] = m_planets[i].body[0];
	}
	p_window->draw(_pixels);

	ImGui::Begin("Debug");
		ImGui::TextUnformatted(m_fpsText.c_str());
		ImGui::TextUnformatted(m_nbPlanets.c_str());

		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Simulation"))
		{
			ImGui::Checkbox("Pause", &m_pause);
			ImGui::DragFloat("Force limit", &Config::maxForce, 0.01f);
			ImGui::DragFloat("Gravity", &Config::gravity, 0.1f);
			ImGui::TreePop();
		}

		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Barnes Hut"))
		{
			ImGui::Checkbox("Use BarnesHut", &m_useBarneHut);
			ImGui::SliderFloat("Threshold compute extern node", &Config::thresholdCompute, 0, 100);
			ImGui::Checkbox("Display Quadtree", &Config::displayQuadtree);
			ImGui::TreePop();
		}	
	ImGui::End();
}