#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

class VertexQuadTree
{
private :
	VertexQuadTree() = default;

public :
	static VertexQuadTree* instance() noexcept;
	std::vector<sf::VertexArray> quads;

private :
	static VertexQuadTree* s_vertexQuadTree;
};