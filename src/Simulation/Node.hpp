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
	void computeForce(Planet* planet);

private :
	void updateMassPosition();
	void insertQuadrant(Planet* planet);
	void getChildPlanet(Node* node, std::vector<Planet*>& planets);

public :
	float mass;
	sf::Vector2f position;
	
	Quad quad;
	Planet* planet;
	std::vector<Planet*> childPlanets;

	std::unique_ptr<Node> nw;
	std::unique_ptr<Node> ne;
	std::unique_ptr<Node> sw;
	std::unique_ptr<Node> se;
	
};