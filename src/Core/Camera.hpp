#pragma once
#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(float speed, const sf::Vector2f& position);
	void manageMovements(float deltaTime);
	const sf::View& getView() noexcept;
	void setSpeed(float speed) noexcept;

private:
	sf::Vector2f m_position;
	float m_speed;
	sf::View m_view;
};