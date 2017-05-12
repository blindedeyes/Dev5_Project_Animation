#pragma once
#include "stdafx.h"

class pipelineManager
{
	HWND mhWnd;
	CComPtr<ID3D11Device> device;
	CComPtr<IDXGISwapChain> swapChain;
	CComPtr<ID3D11DeviceContext> devContext;
	//CComPtr<ID3D11RenderTargetView> backBuffer;
	CComPtr<ID3D11Buffer> triangleVBuffer;
	CComPtr<ID3D11Buffer> transformVBuffer;

	CComPtr<ID3D11Buffer> cbWorldBuffer;

	//CComPtr<ID3D11RenderTargetView> targetView;
	D3D11_VIEWPORT vPort;

	pipelineState default_pipeline;
	pipelineState debug_Pipeline;

	cWorldData bufferData;
	DirectX::XMFLOAT4X4 Camera;

	DirectX::XMFLOAT4X4 transformPos[4];

	bool debugMode=false;
	bool wireframeMode = false;
	DebugObjects debugObjects;
	
	int animatorState=0;
	float aniTimer = 0;
	POINT mPrevPoint;
	//Init
	void InitDepthBuffer(ID3D11Texture2D ** text, int width, int height);

	//TO BE ADDED
	void InitDepthState();
	void InitDepthView(ID3D11Texture2D * texture, ID3D11DepthStencilView ** view);
	void InitShaders();

	void CreateTriangle();
	void CreatePlane();
	void UpdateCamera(float delta_time);
	void UpdateAnimation(float delta_time);
	void AppendDebugBones(Bone b);// , vertex parent);
	void DebugDrawBones(unsigned int ani, unsigned int key, Mesh &m);
	void DrawDebugAnimationBone(unsigned int ani, unsigned int key, Bone b, vertex parent);
	void DrawDebugAnimationBoneTime(unsigned int ani, unsigned int key, Bone b, vertex parent);


public:


	pipelineManager();



	void InitPipeline(HWND hWnd);

	void Update(float deltaTime);
	void ClearBuffers();
	void Drawstate();
	void Swap();
	void DrawPipeLine(const pipelineState &state);
	void DrawDebugObjecs();


	~pipelineManager();
};

