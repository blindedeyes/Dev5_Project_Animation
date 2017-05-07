#pragma once
#include <vector>

struct vertex
{
	union
	{
		struct { float x, y, z, w; };
		float pos[4];
	};
	union
	{
		struct { float r, g, b, a; };
		float color[4];
	};

	vertex() {
	};
	vertex(float _x, float _y, float _z, float _w, float _r, float _g, float _b, float _a)
		: x{ _x }, y{ _y }, z{ _z }, w{ _w }, r{ _r }, g{ _g }, b{ _b }, a{ _a }
	{

	}
};

struct Bone
{
	vertex v;
	std::vector<Bone> children;
};
struct Mesh
{
	//array of vertexes
	std::vector<vertex> verts;
	Bone root;
	//array of tri indexes
	std::vector<unsigned int> indices;
	//int* indices;
};