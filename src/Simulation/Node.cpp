#include "Node.hpp"

Node::Node(Quad quad)
{
	this->quad = quad;
}

void Node::insert(Planet* planet)
{
	if (this->planet == nullptr)
	{
		this->planet = planet;

		sf::Vector2f childSize = this->quad.size /= 2.0f;	
		nw = std::unique_ptr<Node>(new Node({ this->quad.position, childSize }));
		ne = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(childSize.x, 0), childSize }));
		sw = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(0, childSize.y), childSize }));
		se = std::unique_ptr<Node>(new Node({ this->quad.position + sf::Vector2f(childSize.x, childSize.y), childSize }));

		quadTree.reserve(4);
		quadTree.push_back(&nw.get()->quad);
		quadTree.push_back(&ne.get()->quad);
		quadTree.push_back(&sw.get()->quad);
		quadTree.push_back(&se.get()->quad);
	}
}