#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>

#include "Application.hpp"
#include "Sandbox.hpp"
#include "../Config.hpp"

Application* Application::s_app = nullptr;
Application::Application()
{
	p_window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Barnes Hut Simulation");
	ImGui::SFML::Init(*p_window);
}

Application* Application::instance() noexcept
{
	if (s_app == nullptr)
	{
		s_app = new Application();
	}
	return s_app;
}

int Application::run()
{
	Sandbox sandbox(p_window);
	while (p_window->isOpen())
	{
		while (p_window->pollEvent(m_event))
		{
			ImGui::SFML::ProcessEvent(*p_window, m_event);
			if (m_event.type == sf::Event::Closed)
			{
				p_window->close();
				ImGui::SFML::Shutdown();
			}
		}

		m_deltaTime = m_deltaClock.restart();
		ImGui::SFML::Update(*p_window, m_deltaTime);
		sandbox.update(m_deltaTime.asSeconds());
		m_deltaClock.restart();

		p_window->clear();	
		sandbox.draw();
		ImGui::SFML::Render(*p_window);
		p_window->display();
	}

	return 0;
}