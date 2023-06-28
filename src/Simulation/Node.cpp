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
	if (!nw.get() && this->planet)
	{
		const sf::Vector2f childSize = this->quad.size / 2.0f;
		nw = std::unique_ptr<Node>(new Node({ this->quad.position, childSize }));
		ne = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(childSize.x, 0), childSize }));
		sw = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(0, childSize.y), childSize }));
		se = std::unique_ptr<Node>(new Node({ this->quad.position + childSize, childSize }));
		
		if (Config::displayQuadtree)
		{
			auto& quads = VertexQuadTree::instance()->quads;
			quads.emplace_back(nw->quad.vertices);
			quads.emplace_back(ne->quad.vertices);
			quads.emplace_back(sw->quad.vertices);
			quads.emplace_back(se->quad.vertices);
		}
		
		this->insertQuadrant(this->planet);
		this->insertQuadrant(planet);
		this->planet = nullptr;

		this->updateMassPosition();
		return;
	}

	if (!this->planet && !nw.get())
	{
		this->planet = planet;
		this->updateMassPosition();
	}

	if (!this->planet && nw.get())
	{
		this->insertQuadrant(planet);
		this->updateMassPosition();
	}
}

void Node::computeForce(Planet* planet)
{
	if (this->planet == planet || (!this->planet && !this->nw.get()))
	{
		return;
	}

	if (this->planet && !nw.get())
	{
		planet->velocity += Math::force(planet, this->planet);
		return;
	}

	if (this->quad.size.x / Math::distance(planet->body[0].position, this->position) < Config::thresholdCompute)
	{
		planet->velocity += Math::force(planet, this->position, this->mass);
		return;
	}
	
	this->nw.get()->computeForce(planet);
	this->ne.get()->computeForce(planet);
	this->sw.get()->computeForce(planet);
	this->se.get()->computeForce(planet);
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
	this->position.x = 0;
	this->position.y = 0;

	childPlanets.clear();
	this->getChildPlanet(this, childPlanets);
	for (auto& planet : childPlanets)
	{
		this->mass += planet->mass;
	}

	for (auto& planet : childPlanets)
	{
		this->position += planet->body[0].position * planet->mass;
	}

	this->position /= this->mass;
}

void Node::getChildPlanet(Node* node, std::vector<Planet*>& planets)
{
	if (!node)
	{
		return;
	}

	if (node->planet)
	{
		planets.emplace_back(node->planet);
	}
	
	this->getChildPlanet(node->nw.get(), planets);
	this->getChildPlanet(node->ne.get(), planets);
	this->getChildPlanet(node->sw.get(), planets);
	this->getChildPlanet(node->se.get(), planets);
}