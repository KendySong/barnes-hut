#pragma once
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

	Node* nw;
	Node* ne;
	Node* sw;
	Node* se;
};