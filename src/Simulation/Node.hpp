#pragma once
#include <memory>
#include <vector>

#include <SFML/System.hpp>

#include "Quad.hpp"
#include "Planet.hpp"

struct Node
{
public :
	Node() = default;
	Node(Quad quad);
	void insert(Planet* planet);

	float mass;
	sf::Vector2f position;
	Planet* planet;
	Quad quad;

	std::vector<Quad*> quadTree;

	std::unique_ptr<Node> nw;
	std::unique_ptr<Node> ne;
	std::unique_ptr<Node> sw;
	std::unique_ptr<Node> se;
};