#pragma once
#include <vector>
struct Mesh
{
	struct vertex
	{
		union
		{
			float pos[3];
			float x, y, z;
		};
		union
		{
			float pos4;
		};
	};

public:
	std::vector<vertex> verts;
	std::vector<unsigned int> indexes;
};