#include <iostream>

#include "Node.hpp"
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
	
}