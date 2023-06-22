#include "VertexQuadTree.hpp"

VertexQuadTree* VertexQuadTree::s_vertexQuadTree = nullptr;

VertexQuadTree* VertexQuadTree::instance() noexcept
{
	if (s_vertexQuadTree == nullptr)
	{
		s_vertexQuadTree = new VertexQuadTree();
	}

	return s_vertexQuadTree;
}