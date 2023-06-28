#include <iostream>

#include "Node.hpp"
#include "../Config.hpp"
#include "../Math/Math.hpp"
#include "VertexQuadTree.hpp"

Node::Node(Quad quad)
{
	this->planet = nullptr;
	this->quad = quad;

	this->nw = std::unique_ptr<Node>(nullptr);
	this->ne = std::unique_ptr<Node>(nullptr);
	this->sw = std::unique_ptr<Node>(nullptr);
	this->se = std::unique_ptr<Node>(nullptr);
}

void Node::insert(Planet* planet)
{
	if (nw.get() == nullptr && this->planet != nullptr)
	{
		sf::Vector2f childSize = this->quad.size / 2.0f;
		nw = std::unique_ptr<Node>(new Node(Quad(this->quad.position, childSize)));
		ne = std::unique_ptr<Node>(new Node(Quad(this->quad.position + sf::Vector2f(childSize.x, 0), childSize)));
		sw = std::unique_ptr<Node>(new Node(Quad(this->quad.position + sf::Vector2f(0, childSize.y), childSize)));
		se = std::unique_ptr<Node>(new Node(Quad(this->quad.position + childSize, childSize)));
		
		auto& quads = VertexQuadTree::instance()->quads;
		quads.push_back(nw->quad.vertices);
		quads.push_back(ne->quad.vertices);
		quads.push_back(sw->quad.vertices);
		quads.push_back(se->quad.vertices);

		this->insertQuadrant(this->planet);
		this->insertQuadrant(planet);
		this->planet = nullptr;

		this->updateMassPosition();
		return;
	}

	if (this->planet == nullptr && nw.get() == nullptr)
	{
		this->planet = planet;
		this->updateMassPosition();
	}

	if (this->planet == nullptr && nw.get() != nullptr)
	{
		this->insertQuadrant(planet);
		this->updateMassPosition();
	}
}

void Node::computeForce(Planet* planet)
{
	if (this->planet == planet || (this->planet == nullptr && this->nw.get() == nullptr))
	{
		return;
	}

	if (this->planet != nullptr && nw.get() == nullptr)
	{
		planet->velocity += Math::force(planet, this->planet);
		return;
	}

	if (this->canComputeIntern(planet))
	{
		planet->velocity += Math::force(planet, this->position, this->mass);
		return;
	}
	
	this->nw.get()->computeForce(planet);
	this->ne.get()->computeForce(planet);
	this->sw.get()->computeForce(planet);
	this->se.get()->computeForce(planet);
}

bool Node::canComputeIntern(Planet* planet)
{
	return this->quad.size.x / Math::distance(planet->body[0].position, this->position) < Config::thresholdCompute;
}

void Node::insertQuadrant(Planet* planet)
{
	if (nw.get()->quad.contain(planet->body[0].position))
	{
		nw.get()->insert(planet);
		return;
	}

	if (ne.get()->quad.contain(planet->body[0].position))
	{
		ne.get()->insert(planet);
		return;
	}

	if (sw.get()->quad.contain(planet->body[0].position))
	{
		sw.get()->insert(planet);
		return;
	}

	if (se.get()->quad.contain(planet->body[0].position))
	{
		se.get()->insert(planet);
		return;
	}

	std::cout << "Error : Quadrant not found for the planet : " << planet << "\n";
}

void Node::updateMassPosition()
{
	this->mass = 0;
	this->position = sf::Vector2f(0, 0);

	std::vector<Planet*> planets;
	this->getChildPlanet(this, planets);
	for (auto& planet : planets)
	{
		this->mass += planet->mass;
	}

	for (auto& planet : planets)
	{
		this->position += planet->body[0].position * planet->mass;
	}

	this->position /= this->mass;
}

void Node::getChildPlanet(Node* node, std::vector<Planet*>& planets)
{
	if (node == nullptr)
	{
		return;
	}

	if (node->planet != nullptr)
	{
		planets.push_back(node->planet);
	}
	
	this->getChildPlanet(node->nw.get(), planets);
	this->getChildPlanet(node->ne.get(), planets);
	this->getChildPlanet(node->sw.get(), planets);
	this->getChildPlanet(node->se.get(), planets);
}