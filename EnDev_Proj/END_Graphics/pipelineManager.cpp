#pragma once
#include "stdafx.h"
#include "Trivial_VS.csh"
#include "Trivial_PS.csh"



void pipelineManager::InitDepthBuffer(ID3D11Texture2D ** text, int width, int height)
{
	/*D3D11_TEXTURE2D_DESC textureDesc;
	memset(&textureDesc, 0, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
*/
	CD3D11_TEXTURE2D_DESC desc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		lround(width),
		lround(height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);

	

	//CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	device->CreateTexture2D(&desc, NULL, text);
}

void pipelineManager::InitDepthState()
{
}

void pipelineManager::InitDepthView(ID3D11Texture2D * texture, ID3D11DepthStencilView ** view)
{
	//D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	//memset(&desc, 0, sizeof(desc));
	//desc.Format = DXGI_FORMAT_D32_FLOAT;// textureDesc.Format;
	//desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//desc.Texture2D.MipSlice = 0;

	CD3D11_DEPTH_STENCIL_VIEW_DESC desc(D3D11_DSV_DIMENSION_TEXTURE2D);

	device->CreateDepthStencilView(texture, &desc, view);
	
}

void pipelineManager::InitShaders()
{

}

void pipelineManager::CreateTriangle()
{
	//data
	vertex v[3];
	v[0].pos = DirectX::XMFLOAT4(-0.5f, -0.5f,  0.0f,  1.0f);
	v[1].pos = DirectX::XMFLOAT4( 0.0f,  0.5f,  0.0f,  1.0f);
	v[2].pos = DirectX::XMFLOAT4( 0.5f, -0.5f,  0.0f,  1.0f);


	v[0].color = DirectX::XMFLOAT4(1, 0, 0, 1 );
	v[1].color = DirectX::XMFLOAT4(0, 1, 0, 1 );
	v[2].color = DirectX::XMFLOAT4(0, 0, 1, 1 );

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	//setup shaders
	device->CreateVertexShader(&Trivial_VS, sizeof(Trivial_VS), NULL, &default_pipeline.vertex_shader);
	device->CreatePixelShader(&Trivial_PS, sizeof(Trivial_PS), NULL, &default_pipeline.pixel_shader);
	//setup input layout
	device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), Trivial_VS, sizeof(Trivial_VS), &default_pipeline.input_layout.p);

	//vertex buffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = NULL;
	desc.ByteWidth = sizeof(v);


	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = v;

	device->CreateBuffer(&desc, &data, &triangleVBuffer.p);
}

void pipelineManager::CreateTransform()
{
	//setup the transform.
	//transformVBuffer
	vertex verts[6];
	//Z
	verts[0].color = { 0,0,1.0f,0 };
	verts[0].pos = { 0,0,0,1 };
	verts[1].color = { 0,0,1.0f,0 };
	verts[1].pos = { 0,0,1,1 };
	//Y
	verts[2].color = { 0,1.0f,0,0 };
	verts[2].pos = { 0,0,0,1 };
	verts[3].color = { 0,1.0f,0,0 };
	verts[3].pos = { 0,1,0,1 };
	//X
	verts[4].color = { 1.0f,0,0,0 };
	verts[4].pos = { 0,0,0,1 };
	verts[5].color = { 1.0f,0,0,0 };
	verts[5].pos = { 1,0,0,1 };


	//vertex buffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = NULL;
	desc.ByteWidth = sizeof(verts);


	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = verts;

	device->CreateBuffer(&desc, &data, &transformVBuffer.p);

	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(cWorldData), D3D11_BIND_CONSTANT_BUFFER);



	device->CreateBuffer(&constantBufferDesc, &data, &cbWorldBuffer.p);

	DirectX::XMStoreFloat4x4(&transformPos[0], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&transformPos[1], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&transformPos[2], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&transformPos[3], DirectX::XMMatrixIdentity());
}

pipelineManager::pipelineManager()
{
}

void pipelineManager::InitPipeline(HWND hWnd)
{
	srand((unsigned int)std::time(0));
	//Setup d3d11
	mhWnd = hWnd;

	DXGI_SWAP_CHAIN_DESC swapDesc;

	ZeroMemory(&swapDesc, sizeof(swapDesc));

	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = mhWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Windowed = true;

#ifdef DEBUG
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain.p,
		&device.p,
		NULL,
		&devContext.p);
#else
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain,
		&device,
		NULL,
		&devContext);
	
#endif

	//init back buffer
	
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&default_pipeline.BackBuffer.p);

	device->CreateRenderTargetView(default_pipeline.BackBuffer.p, NULL, &default_pipeline.render_target.p);
	//delete pBuffer;

	devContext->OMSetRenderTargets(1, &default_pipeline.render_target.p, NULL);
	RECT rec;
	//GetWindowRect(mhWnd, &rec);
	GetClientRect(mhWnd, &rec);
	//used for viewport
	int width = rec.right - rec.left;
	int height = rec.bottom - rec.top;

	memset(&vPort, 0, sizeof(D3D11_VIEWPORT));
	vPort.TopLeftX = 0;
	vPort.TopLeftY = 0;
	vPort.Width = width;
	vPort.Height = height;
	vPort.MinDepth = 0;
	vPort.MaxDepth = 1;


	float aspectRatio = (float)width / height;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	InitDepthBuffer(&default_pipeline.depthStencilBuffer.p, width, height);
	InitDepthView(default_pipeline.depthStencilBuffer.p, &default_pipeline.depthStencilView.p);

	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);

	DirectX::XMStoreFloat4x4(&bufferData.proj, DirectX::XMMatrixTranspose(perspectiveMatrix));
	static const DirectX::XMVECTORF32 eye = { 1.0f, 3.5f, -2.5f, 0.0f };
	
	static const DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMStoreFloat4x4(&bufferData.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye,at,up)));

	CreateTriangle();
	CreateTransform();
}

void pipelineManager::Update()
{
	//Randomize legecy code
//	if (GetActiveWindow() == mhWnd)
//	{
//		
//		if (GetAsyncKeyState('R'))
//		{
//			DirectX::XMMATRIX mat;
//			mat = DirectX::XMMatrixRotationX(((rand() % 360) * (DirectX::XM_PI / 180.0f)));
//			mat = DirectX::XMMatrixRotationY(((rand() % 360) * (DirectX::XM_PI / 180.0f))) * mat;
//			mat = DirectX::XMMatrixRotationZ(((rand() % 360) * (DirectX::XM_PI / 180.0f))) * mat;
//			mat = mat * DirectX::XMMatrixTranslation(((rand() % 10) - 5) / 5.0f, ((rand() % 10) - 5) / 5.0f, ((rand() % 10) - 5) / 5.0f);
//
//
//			DirectX::XMStoreFloat4x4(&transformPos[0], DirectX::XMMatrixTranspose(mat));
//
////			DirectX::XMMATRIX mat;
//			mat = DirectX::XMMatrixRotationX(((rand() % 360) * (DirectX::XM_PI / 180.0f)));
//			mat = DirectX::XMMatrixRotationY(((rand() % 360) * (DirectX::XM_PI / 180.0f))) * mat;
//			mat = DirectX::XMMatrixRotationZ(((rand() % 360) * (DirectX::XM_PI / 180.0f))) * mat;
//			mat = mat * DirectX::XMMatrixTranslation(((rand() % 10) - 5) / 5.0f, ((rand() % 10) - 5) / 5.0f, ((rand() % 10) - 5) / 5.0f);
//
//
//
//			DirectX::XMStoreFloat4x4(&transformPos[1], DirectX::XMMatrixTranspose(mat));
//
//	//		DirectX::XMMATRIX mat;
//			mat = DirectX::XMMatrixRotationX(((rand() % 360) * (DirectX::XM_PI / 180.0f)));
//			mat = DirectX::XMMatrixRotationY(((rand() % 360) * (DirectX::XM_PI / 180.0f))) * mat;
//			mat = DirectX::XMMatrixRotationZ(((rand() % 360) * (DirectX::XM_PI / 180.0f))) * mat;
//			mat = mat * DirectX::XMMatrixTranslation(((rand() % 10) - 5) / 5.0f, ((rand() % 10) - 5) / 5.0f, ((rand() % 10) - 5) / 5.0f);
//
//
//
//			DirectX::XMStoreFloat4x4(&transformPos[2], DirectX::XMMatrixTranspose(mat));
//
//		//	DirectX::XMMATRIX mat;
//			mat = DirectX::XMMatrixRotationX(((rand() % 360) * (DirectX::XM_PI / 180.0f)));
//			mat = DirectX::XMMatrixRotationY(((rand() % 360) * (DirectX::XM_PI / 180.0f))) * mat;
//			mat = DirectX::XMMatrixRotationZ(((rand() % 360) * (DirectX::XM_PI / 180.0f))) * mat;
//			mat = mat * DirectX::XMMatrixTranslation(((rand() % 10) - 5) / 5.0f, ((rand() % 10) - 5) / 5.0f, ((rand() % 10) - 5) / 5.0f);
//
//
//			DirectX::XMStoreFloat4x4(&transformPos[3], DirectX::XMMatrixTranspose(mat));
//		}
//	}


}

void pipelineManager::ClearBuffers()
{
	float data[4] = { 0.0f, 0.0f,0.0f,0.0f };
	devContext->ClearRenderTargetView(default_pipeline.render_target.p, data);
	devContext->ClearDepthStencilView(default_pipeline.depthStencilView, D3D11_CLEAR_DEPTH,1, 0);
	devContext->OMSetRenderTargets(1, &default_pipeline.render_target.p, default_pipeline.depthStencilView.p);
	devContext->RSSetViewports(1, &vPort);
}

void pipelineManager::Drawstate()
{
	//Draw stuff
	//render default_pipeline
	//devContext->iaset

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	bufferData.world[0] = transformPos[0];
	bufferData.world[1] = transformPos[1];
	bufferData.world[2] = transformPos[2];
	bufferData.world[3] = transformPos[3];
	
	devContext->UpdateSubresource(cbWorldBuffer.p, 0, NULL, &bufferData, 0, 0);

	//Draw the transforms
	devContext->IASetVertexBuffers(0, 1, &transformVBuffer.p, &stride, &offset);
	devContext->VSSetShader(default_pipeline.vertex_shader.p, 0, 0);
	devContext->VSSetConstantBuffers(0, 1, &cbWorldBuffer.p);
	devContext->PSSetShader(default_pipeline.pixel_shader.p, 0, 0);
	devContext->IASetInputLayout(default_pipeline.input_layout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	devContext->DrawInstanced(6, 4, 0, 0);


	DirectX::XMStoreFloat4x4(&bufferData.world[0], DirectX::XMMatrixIdentity());
	devContext->UpdateSubresource(cbWorldBuffer.p, 0, NULL, &bufferData, 0, 0);

	devContext->IASetVertexBuffers(0, 1, &triangleVBuffer.p, &stride, &offset);
	devContext->VSSetShader(default_pipeline.vertex_shader.p, 0, 0);
	devContext->VSSetConstantBuffers(0, 1, &cbWorldBuffer.p);
	devContext->PSSetShader(default_pipeline.pixel_shader.p, 0, 0);
	devContext->IASetInputLayout(default_pipeline.input_layout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//devContext->Draw(3, 0);
	devContext->DrawInstanced(3, 1, 0, 0);
}

void pipelineManager::Swap()
{
	swapChain->Present(0,0);
}


pipelineManager::~pipelineManager()
{

	devContext->ClearState();

	//default_pipeline.depthStencilState.Release();
	
	

	//Release
	triangleVBuffer.Release();
	//backBuffer.Release();

	default_pipeline.pixel_shader.Release();
	default_pipeline.input_layout.Release();
	default_pipeline.vertex_shader.Release();
	default_pipeline.depthStencilView.Release();
	default_pipeline.depthStencilBuffer.Release();
	default_pipeline.render_target.Release();
	default_pipeline.BackBuffer.Release();
	swapChain.Release();
	devContext.Release();
	
	device.Release();

	default_pipeline.depthStencilBuffer.p = nullptr;;
	default_pipeline.depthStencilState.p = nullptr;;
	default_pipeline.input_layout.p = nullptr;
	default_pipeline.pixel_shader.p = nullptr;;
	default_pipeline.render_target.p = nullptr;;
	default_pipeline.vertex_shader.p = nullptr;;
	triangleVBuffer.p =  nullptr;
	//backBuffer.p = nullptr;
	swapChain.p = nullptr;
	devContext.p = nullptr;
	device.p = nullptr;
	
}
