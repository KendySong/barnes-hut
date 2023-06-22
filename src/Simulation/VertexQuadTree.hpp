#pragma once
#include <vector>

class VertexQuadTree
{
private :
	VertexQuadTree();

public :
	static VertexQuadTree* instance() noexcept;


private :
	static VertexQuadTree* s_vertexQuadTree;
	std::vector<>
};