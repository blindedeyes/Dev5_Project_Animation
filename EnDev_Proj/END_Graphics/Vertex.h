#pragma once
#include "stdafx.h"

//struct vertex
//{
//	DirectX::XMFLOAT4 pos;
//	DirectX::XMFLOAT4 color;
//};

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
	vertex() {};
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
	//std::vector<vertex> bones;
	Bone root;
	//array of tri indexes
	std::vector<unsigned int> indices;
	//int* indices;
};

struct RenderObject
{
	DirectX::XMFLOAT4X4 worldMat;

	int instanceCnt = 1;
	Mesh mesh;

	ID3D11Buffer * vertexBuffer = nullptr;
	ID3D11Buffer * indexBuffer = nullptr;

	void createBuffer(ID3D11Device * device)
	{
		D3D11_BUFFER_DESC desc;

		ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = NULL;
		desc.ByteWidth = sizeof(vertex) * mesh.verts.size();


		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = mesh.verts.data();

		device->CreateBuffer(&desc, &data, &vertexBuffer);
		////D3D11_BUFFER_DESC desc;

		//ZeroMemory(&desc, sizeof(desc));

		//desc.Usage = D3D11_USAGE_IMMUTABLE;
		//desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//desc.CPUAccessFlags = NULL;
		//desc.ByteWidth = sizeof(vertex) * mesh.bones.size();


		////D3D11_SUBRESOURCE_DATA data;
		//data.pSysMem = mesh.bones.data();

		//device->CreateBuffer(&desc, &data, &debugVBuffer);
	}
	void createIndex(ID3D11Device * device)
	{
		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = mesh.indices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) *  mesh.indices.size(), D3D11_BIND_INDEX_BUFFER);
		device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

	}

	RenderObject()
	{

	}
	~RenderObject()
	{
		if (indexBuffer) indexBuffer->Release();
		if (vertexBuffer) vertexBuffer->Release();
	}
};

struct DebugObjects
{
	ID3D11Buffer * vertexBuffer = nullptr;
	vertex * lineVerts=nullptr;
	unsigned int MaxCnt;
	unsigned int CurrentCount=0;
	DebugObjects(unsigned int MaxLineCnt = 1000)
	{
		MaxCnt = MaxLineCnt*2;
		lineVerts = new vertex[MaxLineCnt*2];
	}
	~DebugObjects()
	{
		delete lineVerts;
	}

	void createBuffer(ID3D11Device * device)
	{
		D3D11_BUFFER_DESC desc;



		ZeroMemory(&desc, sizeof(desc));

		desc.ByteWidth = sizeof(vertex) * MaxCnt; //total elements
		desc.StructureByteStride = sizeof(vertex); //element size
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//desc.Usage = D3D11_USAGE_IMMUTABLE;
		//desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//desc.CPUAccessFlags = NULL;
		//desc.ByteWidth = sizeof(vertex) * mesh.verts.size();


		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = lineVerts;

		device->CreateBuffer(&desc, &data, &vertexBuffer);
	}
	void AddLine(vertex start, vertex end)
	{
		if (CurrentCount > MaxCnt - 2) return;
		lineVerts[CurrentCount] = start;
		lineVerts[CurrentCount + 1] = end;
		CurrentCount += 2;
	}

};
struct cWorldData
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMFLOAT4X4 world[4];

	cWorldData()
	{
		DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&world[0], DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&world[1], DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&world[2], DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&world[3], DirectX::XMMatrixIdentity());
	}
};