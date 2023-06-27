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
	void insertQuadrant(Planet* planet);
	void updateMassPosition();
	
public :
	float mass;
	sf::Vector2f position;
	Planet* planet;
	Quad quad;

	std::unique_ptr<Node> nw;
	std::unique_ptr<Node> ne;
	std::unique_ptr<Node> sw;
	std::unique_ptr<Node> se;
};