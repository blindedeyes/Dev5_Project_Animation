#pragma once
#include "stdafx.h"
struct pipelineState
{
	CComPtr<ID3D11Texture2D> BackBuffer;
	CComPtr<ID3D11Texture2D> depthStencilBuffer;
	CComPtr<ID3D11DepthStencilState> depthStencilState;
	CComPtr<ID3D11DepthStencilView> depthStencilView;
	
	CComPtr<ID3D11RenderTargetView> render_target;
	CComPtr<ID3D11RasterizerState> rasterState;
	
	//The Input layout for these objects
	CComPtr<ID3D11InputLayout> input_layout;
	//The shaders for these objects
	CComPtr<ID3D11PixelShader > pixel_shader;
	CComPtr<ID3D11VertexShader> vertex_shader;

	//The list of objects that use these shaders
	std::vector<RenderObject> rendObjects;
};
