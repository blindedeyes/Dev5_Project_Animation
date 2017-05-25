#pragma once
#include "stdafx.h"

//struct vertex
//{
//	DirectX::XMFLOAT4 pos;
//	DirectX::XMFLOAT4 color;
//};

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
			float m11, m12, m13, m14,
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

struct shaderAni {
	DirectX::XMMATRIX keyframe[50];
};
struct shaderBone {
	shaderAni anim[15];
};
struct cShaderBones
{
	unsigned int aniID;
	unsigned int KeyFrame;
	float ratio;
	shaderBone bones[50];
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
	//int indices;
};

struct RenderObject
{
	DirectX::XMFLOAT4X4 worldMat;

	int instanceCnt = 1;
	int animID = 0;
	int animKeyID = 0;
	Mesh mesh;

	ID3D11Buffer * vertexBuffer = nullptr;
	ID3D11Buffer * indexBuffer = nullptr;
	//ID3D11Buffer * wireIndexBuffer = nullptr;

	CComPtr<ID3D11Buffer> cbBoneBuffer;
	cShaderBones *bufferBoneData = nullptr;

	//purpose: populate vert bind space pos
	void invBindPosition() {
		for (unsigned int i = 0; i < mesh.bones.size(); i++) {
			//the the first key of every ani, and inv the mat
			for (unsigned int ani = 0; ani < mesh.bones[i].Anims.size(); ++ani) {
				//first key, inv it.
				//only if we have a key
				if (mesh.bones[i].Anims[ani].keys.size()) {
					DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&DirectX::XMFLOAT4X4(mesh.bones[i].Anims[ani].keys[0].data));
					mat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, mat));
					DirectX::XMFLOAT4X4 temp;
					DirectX::XMStoreFloat4x4(&temp, mat);
					float* data = mesh.bones[i].Anims[ani].keys[0].data;
					//f*@#! hard code
					data[0] = temp._11;
					data[1] = temp._12;
					data[2] = temp._13;
					data[3] = temp._14;
					data[4] = temp._21;
					data[5] = temp._22;
					data[6] = temp._23;
					data[7] = temp._24;
					data[8] = temp._31;
					data[9] = temp._32;
					data[10] = temp._33;
					data[11] = temp._34;
					data[12] = temp._41;
					data[13] = temp._42;
					data[14] = temp._43;
					data[16] = temp._44;

					using namespace DirectX;
					for (unsigned int key = 1; mesh.bones[i].Anims[ani].keys.size(); ++key) {
						temp = XMFLOAT4X4(mesh.bones[i].Anims[ani].keys[key].data);

						mat = DirectX::XMLoadFloat4x4(&temp);
						mat = DirectX::XMMatrixTranspose((nullptr, mat));
						
						DirectX::XMStoreFloat4x4(&temp, mat);
						float* data = mesh.bones[i].Anims[ani].keys[key].data;
						//f*@#! hard code
						data[0] = temp._11;
						data[1] = temp._12;
						data[2] = temp._13;
						data[3] = temp._14;
						data[4] = temp._21;
						data[5] = temp._22;
						data[6] = temp._23;
						data[7] = temp._24;
						data[8] = temp._31;
						data[9] = temp._32;
						data[10] = temp._33;
						data[11] = temp._34;
						data[12] = temp._41;
						data[13] = temp._42;
						data[14] = temp._43;
						data[16] = temp._44;
					}
				}
			}
		}
	}
	void createBuffer(ID3D11Device * device)
	{
		invBindPosition();
		D3D11_BUFFER_DESC desc;

		ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = NULL;
		desc.ByteWidth = sizeof(vertex) * mesh.verts.size();


		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = mesh.verts.data();

		device->CreateBuffer(&desc, &data, &vertexBuffer);

		if (mesh.bones.size()) {
			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(cShaderBones), D3D11_BIND_CONSTANT_BUFFER);
			device->CreateBuffer(&constantBufferDesc, NULL, &cbBoneBuffer.p);
			bufferBoneData = new cShaderBones();
			bufferBoneData->aniID = 0;
			bufferBoneData->KeyFrame = 0;
			bufferBoneData->ratio = 0;
			for (unsigned int i = 0; i < mesh.bones.size(); ++i) {
				for (unsigned int ani = 0; ani < mesh.bones[i].Anims.size(); ++ani) {
					for (unsigned int key = 0; key < mesh.bones[i].Anims[ani].keys.size(); ++key) {
						bufferBoneData->bones[i].anim[ani].keyframe[key] = DirectX::XMLoadFloat4x4(&DirectX::XMFLOAT4X4(mesh.bones[i].Anims[ani].keys[key].data));
					}
				}
			}
		}
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

		/*
		unsigned int i;
		for (i = 0; i < mesh.indices.size(); i += 3) {
			//print the faces in wire frame, 0 to 1, 1 to 2, 2 to 0 should work?
			mesh.wireIndices.push_back(mesh.indices[i]);
			mesh.wireIndices.push_back(mesh.indices[i+1]);
			mesh.wireIndices.push_back(mesh.indices[i+1]);
			mesh.wireIndices.push_back(mesh.indices[i+2]);
			mesh.wireIndices.push_back(mesh.indices[i+2]);
			mesh.wireIndices.push_back(mesh.indices[i]);
		}

		D3D11_SUBRESOURCE_DATA wireframeindexBufferData = { 0 };
		wireframeindexBufferData.pSysMem = mesh.wireIndices.data();
		wireframeindexBufferData.SysMemPitch = 0;
		wireframeindexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC wireframeindexBufferDesc(sizeof(unsigned int) *  mesh.wireIndices.size(), D3D11_BIND_INDEX_BUFFER);
		device->CreateBuffer(&wireframeindexBufferDesc, &wireframeindexBufferData, &wireIndexBuffer);
		*/
	}

	RenderObject()
	{

	}
	~RenderObject()
	{
		//delete bufferBoneData;
	}

	void Cleanup() {
		cbBoneBuffer.Release();
		vertexBuffer->Release();
		indexBuffer->Release();
		delete bufferBoneData;
	}
	float getKeyFrameTotal() {
		unsigned int prevFrame = animKeyID - 1;
		if (prevFrame == 0)
			prevFrame = mesh.bones[0].Anims[animID].keys.size() - 1;
		if (prevFrame < animKeyID) {
			return (mesh.bones[0].Anims[animID].keys[animKeyID].KeyTime - mesh.bones[0].Anims[animID].keys[prevFrame].KeyTime);
		}
		else {
			return (mesh.bones[0].Anims[animID].keys[animKeyID].KeyTime);
		}
	}
};

struct DebugObjects
{
	ID3D11Buffer * vertexBuffer = nullptr;
	vertex * lineVerts = nullptr;
	unsigned int MaxCnt;
	unsigned int CurrentCount = 0;

	DebugObjects(unsigned int MaxLineCnt = 1000)
	{
		MaxCnt = MaxLineCnt * 2;
		lineVerts = new vertex[MaxLineCnt * 2];
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
	void ResetLines() {
		CurrentCount = 0;
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