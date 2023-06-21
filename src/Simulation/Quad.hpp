#pragma once
#include <SFML/Graphics.hpp>

struct Quad
{
public :
	Quad() = default;
	Quad(sf::Vector2f position, sf::Vector2f size);
	void construct() noexcept;

	sf::VertexArray vertices;
	sf::Vector2f position;
	sf::Vector2f size;
};