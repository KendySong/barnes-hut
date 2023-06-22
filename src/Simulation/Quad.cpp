#include "Quad.hpp"

Quad::Quad(sf::Vector2f position, sf::Vector2f size)
{
	this->position = position;
	this->size = size;
	this->construct();
}

void Quad::construct() noexcept
{
	vertices = sf::VertexArray(sf::LineStrip, 5);
	vertices[0] = sf::Vertex(position, sf::Color::White);
	vertices[1] = sf::Vertex(position + sf::Vector2f(size.x, 0), sf::Color::White);
	vertices[2] = sf::Vertex(position + size, sf::Color::White);
	vertices[3] = sf::Vertex(position + sf::Vector2f(0, size.y), sf::Color::White);
	vertices[4] = sf::Vertex(position, sf::Color::White);
}

bool Quad::contain(sf::Vector2f point) noexcept
{
	return point.x >= position.x && point.x <= position.x + size.x && point.y >= position.y && point.y <= position.y + size.y;
}