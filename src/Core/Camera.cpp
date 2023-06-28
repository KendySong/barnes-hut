#include "Camera.hpp"
#include "../Config.hpp"

Camera::Camera(float speed, const sf::Vector2f& position) : m_speed(speed), m_position(position)
{
	m_view = sf::View(m_position, sf::Vector2f(Config::Width, Config::Height));
}

void Camera::manageMovements(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_position.y -= m_speed * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_position.x -= m_speed * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_position.y += m_speed * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_position.x += m_speed * deltaTime;
	}

	m_view.setCenter(m_position);
}

const sf::View& Camera::getView() noexcept
{
	return m_view;
}

void Camera::setSpeed(float speed) noexcept
{
	m_speed = speed;
}