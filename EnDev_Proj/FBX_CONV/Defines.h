#pragma once
#include <vector>

struct vertex
{
	unsigned int bneCnt = 0;
	unsigned int boneID[4];
	float boneWeight[4];

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

	vertex() : color{ 1,1,1,1 }, pos{ 0,0,0,0 } {
	};
	vertex(float _x, float _y, float _z, float _w, float _r, float _g, float _b, float _a)
		: x{ _x }, y{ _y }, z{ _z }, w{ _w }, r{ _r }, g{ _g }, b{ _b }, a{ _a }
	{

	}
};

struct AnimKey {
	union {
		struct {
			float m11, m12, m13, m14,
				m21, m22, m23, m24,
				m31, m32, m33, m34,
				m41, m42, m43, m44;
		};
		float data[16];
	};
	float KeyTime;
};

struct Animation {
	std::vector<AnimKey> keys;
};

struct Bone
{
	vertex v;
	union {
		struct {
			float m11, m12,m13, m14,
				m21, m22, m23, m24,
				m31, m32, m33, m34,
				m41, m42, m43, m44;
		};
		float matrix[16];
	};
	Bone * parent = nullptr;
	std::vector<Bone> children;
	std::vector<Animation> Anims;
};

struct Mesh
{
	//array of vertexes
	std::vector<vertex> verts;
	Bone root;
	//array of tri indexes
	std::vector<unsigned int> indices;
	//std::vector<unsigned int> wireIndices;

	std::vector<Bone> bones;
	//int* indices;
};