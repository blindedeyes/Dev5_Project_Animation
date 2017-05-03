#pragma once
#include "stdafx.h"
struct vertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 color;
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