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
	//Debug at 7 stars bug with srand(0)
	if (nw.get() == nullptr && this->planet != nullptr)
	{
		sf::Vector2f childSize = this->quad.size /= 2.0f;
		nw = std::unique_ptr<Node>(new Node({ this->quad.position, childSize }));
		ne = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(childSize.x, 0), childSize }));
		sw = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(0, childSize.y), childSize }));
		se = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(childSize.x, childSize.y), childSize }));

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

	/*
	if (this->planet == nullptr)
	{
		this->planet = planet;
		this->updateMassPosition();
		return;
	}

	if (this->nw.get() != nullptr)
	{
		this->insertQuadrant(planet);
		this->updateMassPosition();
		return;
	}

	if (nw.get() == nullptr && this->planet != nullptr)
	{
		sf::Vector2f childSize = this->quad.size /= 2.0f;
		nw = std::unique_ptr<Node>(new Node({ this->quad.position, childSize }));
		ne = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(childSize.x, 0), childSize }));
		sw = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(0, childSize.y), childSize }));
		se = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(childSize.x, childSize.y), childSize }));
		
		VertexQuadTree::instance()->quads.push_back(nw->quad.vertices);
		VertexQuadTree::instance()->quads.push_back(ne->quad.vertices);
		VertexQuadTree::instance()->quads.push_back(sw->quad.vertices);
		VertexQuadTree::instance()->quads.push_back(se->quad.vertices);

		this->insertQuadrant(this->planet);
		this->insertQuadrant(planet);
		this->planet = nullptr;

		this->updateMassPosition();
		return;
	}
	*/
}

void Node::insertQuadrant(Planet* planet)
{
	if (nw.get()->quad.contain(planet->body[0].position))
	{
		nw.get()->insert(planet);
	}

	if (ne.get()->quad.contain(planet->body[0].position))
	{
		ne.get()->insert(planet);
	}

	if (sw.get()->quad.contain(planet->body[0].position))
	{
		sw.get()->insert(planet);
	}

	if (se.get()->quad.contain(planet->body[0].position))
	{
		se.get()->insert(planet);
	}
}

void Node::updateMassPosition()
{
	if (nw.get() == nullptr)
	{
		this->mass = this->planet->mass;
		this->position = this->planet->body[0].position;
		return;
	}

	this->mass = 0;
	this->position = sf::Vector2f(0, 0);

	sf::Vector2f averagePosition;
	std::vector<Planet*>* planets = Node::getPlanets(this);
	for (size_t i = 0; i < planets->size(); i++)
	{
		this->mass += (*planets)[i]->mass;
		const sf::Vector2f& position = (*planets)[i]->body[0].position;
		
		averagePosition.x += position.x * (*planets)[i]->mass;
		averagePosition.y += position.y * (*planets)[i]->mass;
	}

	this->position = averagePosition / this->mass;
	delete planets;
}

std::vector<Planet*>* Node::getPlanets(Node* node)
{
	std::vector<Planet*>* planets = new std::vector<Planet*>;
	Node::traverse(planets, node);
	return planets;
}

void Node::traverse(std::vector<Planet*>* planets, Node* node)
{
	if (node == nullptr)
	{
		return;
	}

	if (node->planet != nullptr)
	{
		planets->push_back(node->planet);
	}

	Node::traverse(planets, node->nw.get());
	Node::traverse(planets, node->ne.get());
	Node::traverse(planets, node->sw.get());
	Node::traverse(planets, node->se.get());
}