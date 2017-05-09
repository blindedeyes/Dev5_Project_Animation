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

void pipelineManager::AppendDebugBones(Bone b, vertex parent)
{
	parent.x += b.v.x;
	parent.y += b.v.y;
	parent.z += b.v.z;

	for (unsigned int j = 0; j < b.children.size(); j++)
	{
		vertex end = b.children[j].v;
		end.x += parent.x;
		end.y += parent.y;
		end.z += parent.z;
		/*
		if (b.parent)
		{
			end.x += parent.x;
			end.y += parent.y;
			end.z += parent.z;
		}
		else
		{

		}
		*/

		debugObjects.AddLine(parent, end);
		AppendDebugBones(b.children[j], parent);
	}
}
void pipelineManager::CreateTriangle()
{
	//data
	vertex v[3];
	//v[0].pos = DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f);
	//v[1].pos = DirectX::XMFLOAT4(0.0f, 0.5f, 0.0f, 1.0f);
	//v[2].pos = DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f, 1.0f);
	//
	v[0] = vertex(-0.5f, -0.5f, 0.0f, 1.0f, 1, 0, 0, 1);
	v[1] = vertex(0.0f, 0.5f, 0.0f, 1.0f, 0, 1, 0, 1);
	v[2] = vertex(0.5f, -0.5f, 0.0f, 1.0f, 0, 0, 1, 1);


	//v[0].color = DirectX::XMFLOAT4(1, 0, 0, 1);
	//v[1].color = DirectX::XMFLOAT4(0, 1, 0, 1);
	//v[2].color = DirectX::XMFLOAT4(0, 0, 1, 1);

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


	bFBX_CONVERT::clsFBXWrapper obj;
	char c = obj.test();
	std::cout << (c);

	//std::vector<Mesh> meshes = obj.LoadFBXFile("battleMage.fbx");
	std::vector<Mesh> meshes = obj.LoadFBXFile("Teddy_Run.fbx");
	
	//meshes[0]
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		if (meshes[i].verts.size() > 0)
		{
			RenderObject ro;
			ro.mesh = meshes[i];
			default_pipeline.rendObjects.push_back(ro);
		}
		else
		{
			AppendDebugBones(meshes[i].root, vertex(0, 0, 0, 0, 1, 1, 1, 1));
		}
	}
	for (unsigned int i = 0; i < default_pipeline.rendObjects.size(); ++i)
	{

		default_pipeline.rendObjects[i].createBuffer(device);
		default_pipeline.rendObjects[i].createIndex(device);
	}
	//vertex buffer
	debugObjects.createBuffer(device);


}

void pipelineManager::CreatePlane()
{
#if 0
	//setup the transform.
	//transformVBuffer
	vertex verts[6];
	////Z
	//verts[0].color = { 0.0f, 0.0f, 1.0f, 0.0f };
	//verts[0].pos = { 0,0,0,1 };
	verts[0] = vertex(0, 0, 0, 1, 0, 0, 1, 0);
	//verts[1].color = { 0,0,1.0f,0 };
	//verts[1].pos = { 0,0,1,1 };
	verts[1] = vertex(0, 0, 1, 1, 0, 0, 1, 0);

	////Y
	//verts[2].color = { 0,1.0f,0,0 };
	//verts[2].pos = { 0,0,0,1 };
	verts[2] = vertex(0, 0, 0, 1, 0, 1, 0, 0);

	//verts[3].color = { 0,1.0f,0,0 };
	//verts[3].pos = { 0,1,0,1 };
	verts[3] = vertex(0, 1, 0, 1, 0, 1, 0, 0);
	////X
	//verts[4].color = { 1.0f,0,0,0 };
	//verts[4].pos = { 0,0,0,1 };
	verts[4] = vertex(0, 0, 0, 1, 1, 0, 0, 0);
	//verts[5].color = { 1.0f,0,0,0 };
	//verts[5].pos = { 1,0,0,1 };
	verts[5] = vertex(1, 0, 0, 1, 1, 0, 0, 0);


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

#endif
	//setup the transform.
	//transformVBuffer
	vertex verts[4];
	////Z
	//verts[0].color = { 0.0f, 0.0f, 1.0f, 0.0f };
	//verts[0].pos = { 0,0,0,1 };
	verts[0] = vertex(1, 0, 1, 1, 1, 1, 1, 1);
	//verts[1].color = { 0,0,1.0f,0 };
	//verts[1].pos = { 0,0,1,1 };
	verts[1] = vertex(-1, 0, 1, 1, 1, 1, 1, 1);

	////Y
	//verts[2].color = { 0,1.0f,0,0 };
	//verts[2].pos = { 0,0,0,1 };
	verts[2] = vertex(-1, 0, -1, 1, 1, 1, 1, 1);

	//verts[3].color = { 0,1.0f,0,0 };
	//verts[3].pos = { 0,1,0,1 };
	verts[3] = vertex(1, 0, -1, 1, 1, 1, 1, 1);

	RenderObject ro;

	for (unsigned int i = 0; i < 6; ++i)
		ro.mesh.verts.push_back(verts[i]);

	ro.mesh.indices.push_back(0);
	ro.mesh.indices.push_back(2);
	ro.mesh.indices.push_back(1);
	ro.mesh.indices.push_back(3);
	ro.mesh.indices.push_back(2);
	ro.mesh.indices.push_back(0);

	default_pipeline.rendObjects.push_back(ro);
	default_pipeline.rendObjects[default_pipeline.rendObjects.size()-1].createBuffer(device);
	default_pipeline.rendObjects[default_pipeline.rendObjects.size()-1].createIndex(device);
	
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(cWorldData), D3D11_BIND_CONSTANT_BUFFER);



	device->CreateBuffer(&constantBufferDesc, NULL, &cbWorldBuffer.p);

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

	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 1000.0f);

	DirectX::XMStoreFloat4x4(&bufferData.proj, DirectX::XMMatrixTranspose(perspectiveMatrix));
	static const DirectX::XMVECTORF32 eye = { 1.0f, 3.5f, -2.5f, 0.0f };

	static const DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&bufferData.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, at, up)));
	DirectX::XMStoreFloat4x4(&Camera, DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixLookAtLH(eye, at, up)));

	D3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	desc.FillMode = D3D11_FILL_SOLID;
	device->CreateRasterizerState(&desc, &default_pipeline.rasterState.p);
	desc.FillMode = D3D11_FILL_WIREFRAME;
	device->CreateRasterizerState(&desc, &default_pipeline.debugRasterState.p);

	CreateTriangle();
	CreatePlane();
}

void pipelineManager::Update(float deltaTime)
{
	//Randomize legecy code
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

	if (GetActiveWindow() == mhWnd) {
		//UpdateCamera(.001f);
		UpdateCamera(deltaTime);

		if (GetAsyncKeyState(VK_RETURN) & 0x01)
			debugMode = !debugMode;

		if (GetAsyncKeyState(VK_SHIFT) & 0x01)
			wireframeMode = !wireframeMode;
	}
	

}

void pipelineManager::UpdateCamera(float delta_time)
{
	const float moveSpd = 10.0f;

	if (GetAsyncKeyState('W'))
	{
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(0.0f, 0.0f, moveSpd * delta_time);
		DirectX::XMMATRIX temp_camera = DirectX::XMLoadFloat4x4(&Camera);
		DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(translation, temp_camera);
		DirectX::XMStoreFloat4x4(&Camera, result);
	}
	if (GetAsyncKeyState('S'))
	{
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(0.0f, 0.0f, -moveSpd * delta_time);
		DirectX::XMMATRIX temp_camera = DirectX::XMLoadFloat4x4(&Camera);
		DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(translation, temp_camera);
		DirectX::XMStoreFloat4x4(&Camera, result);
	}
	if (GetAsyncKeyState('A'))
	{
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(-moveSpd * delta_time, 0.0f, 0.0f);
		DirectX::XMMATRIX temp_camera = DirectX::XMLoadFloat4x4(&Camera);
		DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(translation, temp_camera);
		DirectX::XMStoreFloat4x4(&Camera, result);
	}
	if (GetAsyncKeyState('D'))
	{
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(moveSpd * delta_time, 0.0f, 0.0f);
		DirectX::XMMATRIX temp_camera = DirectX::XMLoadFloat4x4(&Camera);
		DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(translation, temp_camera);
		DirectX::XMStoreFloat4x4(&Camera, result);
	}
	if (GetAsyncKeyState('X'))
	{
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(0.0f, -moveSpd * delta_time, 0.0f);
		DirectX::XMMATRIX temp_camera = DirectX::XMLoadFloat4x4(&Camera);
		DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(translation, temp_camera);
		DirectX::XMStoreFloat4x4(&Camera, result);
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(0.0f, moveSpd * delta_time, 0.0f);
		DirectX::XMMATRIX temp_camera = DirectX::XMLoadFloat4x4(&Camera);
		DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(translation, temp_camera);
		DirectX::XMStoreFloat4x4(&Camera, result);
	}

	POINT curPoint;
	GetCursorPos(&curPoint);
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		float rotSpd = 20.0f;
		float dx = curPoint.x - mPrevPoint.x;
		float dy = curPoint.y - mPrevPoint.y;

		DirectX::XMFLOAT4 pos = DirectX::XMFLOAT4(Camera._41, Camera._42, Camera._43, Camera._44);

		Camera._41 = 0;
		Camera._42 = 0;
		Camera._43 = 0;

		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(dy * rotSpd * delta_time);
		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(dx * rotSpd * delta_time);

		DirectX::XMMATRIX temp_camera = DirectX::XMLoadFloat4x4(&Camera);
		temp_camera = XMMatrixMultiply(rotX, temp_camera);
		temp_camera = XMMatrixMultiply(temp_camera, rotY);

		DirectX::XMStoreFloat4x4(&Camera, temp_camera);

		Camera._41 = pos.x;
		Camera._42 = pos.y;
		Camera._43 = pos.z;
	}
	mPrevPoint = curPoint;
}

void pipelineManager::ClearBuffers()
{
	float data[4] = { 0.0f, 0.0f,0.0f,0.0f };
	devContext->ClearRenderTargetView(default_pipeline.render_target.p, data);
	devContext->ClearDepthStencilView(default_pipeline.depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	devContext->OMSetRenderTargets(1, &default_pipeline.render_target.p, default_pipeline.depthStencilView.p);
	devContext->RSSetViewports(1, &vPort);
}

void pipelineManager::Drawstate()
{
	//Draw stuff
	//render default_pipeline
	//devContext->iaset

	//UINT stride = sizeof(vertex);
	//UINT offset = 0;
	//bufferData.world[0] = transformPos[0];
	//bufferData.world[1] = transformPos[1];
	//bufferData.world[2] = transformPos[2];
	//bufferData.world[3] = transformPos[3];

	//devContext->UpdateSubresource(cbWorldBuffer.p, 0, NULL, &bufferData, 0, 0);
	DirectX::XMStoreFloat4x4(&bufferData.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&Camera))));

	////Draw the transforms
	//devContext->IASetVertexBuffers(0, 1, &transformVBuffer.p, &stride, &offset);
	//devContext->VSSetShader(default_pipeline.vertex_shader.p, 0, 0);
	//devContext->VSSetConstantBuffers(0, 1, &cbWorldBuffer.p);
	//devContext->PSSetShader(default_pipeline.pixel_shader.p, 0, 0);
	//devContext->IASetInputLayout(default_pipeline.input_layout);
	//devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//devContext->DrawInstanced(6, 4, 0, 0);


	//DirectX::XMStoreFloat4x4(&bufferData.world[0], DirectX::XMMatrixIdentity());
	//devContext->UpdateSubresource(cbWorldBuffer.p, 0, NULL, &bufferData, 0, 0);

	//devContext->IASetVertexBuffers(0, 1, &triangleVBuffer.p, &stride, &offset);
	//devContext->VSSetShader(default_pipeline.vertex_shader.p, 0, 0);
	//devContext->VSSetConstantBuffers(0, 1, &cbWorldBuffer.p);
	//devContext->PSSetShader(default_pipeline.pixel_shader.p, 0, 0);
	//devContext->IASetInputLayout(default_pipeline.input_layout);
	//devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////devContext->Draw(3, 0);
	//devContext->DrawInstanced(10024, 1, 0, 0);
	DrawPipeLine(default_pipeline);
	if (debugMode)
	{
		DrawDebugObjecs();
	}
	//DrawDebugPipeline(debug_Pipeline);
}

void pipelineManager::DrawPipeLine(const pipelineState &state)
{
	UINT stride = sizeof(vertex);
	UINT offset = 0;

	if (wireframeMode) 
		devContext->RSSetState(state.debugRasterState.p);
	else 
		devContext->RSSetState(state.rasterState.p);

	DirectX::XMStoreFloat4x4(&bufferData.world[0], DirectX::XMMatrixIdentity());
	devContext->UpdateSubresource(cbWorldBuffer.p, 0, NULL, &bufferData, 0, 0);

	devContext->VSSetShader(state.vertex_shader.p, 0, 0);
	devContext->VSSetConstantBuffers(0, 1, &cbWorldBuffer.p);
	devContext->PSSetShader(state.pixel_shader.p, 0, 0);
	devContext->IASetInputLayout(state.input_layout);

	//devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < state.rendObjects.size(); ++i)
	{
		devContext->IASetVertexBuffers(0, 1, &state.rendObjects[i].vertexBuffer, &stride, &offset);
		devContext->IASetIndexBuffer(state.rendObjects[i].indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//devContext->IASetIndexBuffer(state.rendObjects[i].wireIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//devContext->DrawInstanced(6, 4, 0, 0);
		devContext->DrawIndexedInstanced(state.rendObjects[i].mesh.indices.size(), state.rendObjects[i].instanceCnt, 0, 0, 0);
		//devContext->DrawIndexedInstanced(state.rendObjects[i].mesh.wireIndices.size(), state.rendObjects[i].instanceCnt, 0, 0, 0);
	}
}



void pipelineManager::DrawDebugObjecs()
{
	if (debugObjects.CurrentCount == 0) return;
	devContext->ClearDepthStencilView(default_pipeline.depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

	D3D11_MAPPED_SUBRESOURCE ms1;
	devContext->Map(debugObjects.vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms1);   // map the buffer
	memcpy(ms1.pData, debugObjects.lineVerts, sizeof(vertex)*debugObjects.CurrentCount);
	// copy the data
	//context->Map(m_vertexParticleBuffer.p, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);   // map the buffer
	//memcpy(ms2.pData, ms1.pData, sizeof(particleData));           // copy the data
	devContext->Unmap(debugObjects.vertexBuffer, NULL);
	//context->Unmap(m_vertexParticleBuffer.p, NULL);

	UINT stride = sizeof(vertex);
	UINT offset = 0;

	DirectX::XMStoreFloat4x4(&bufferData.world[0], DirectX::XMMatrixIdentity());

	devContext->VSSetShader(default_pipeline.vertex_shader.p, 0, 0);
	devContext->VSSetConstantBuffers(0, 1, &cbWorldBuffer.p);
	devContext->PSSetShader(default_pipeline.pixel_shader.p, 0, 0);
	devContext->IASetInputLayout(default_pipeline.input_layout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	devContext->IASetVertexBuffers(0, 1, &debugObjects.vertexBuffer, &stride, &offset);
	//devContext->IASetIndexBuffer(state.rendObjects[0].indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//devContext->DrawInstanced(6, 4, 0, 0);
	devContext->Draw(debugObjects.CurrentCount, 0);

}

void pipelineManager::Swap()
{
	swapChain->Present(0, 0);
}


pipelineManager::~pipelineManager()
{

	devContext->ClearState();

	//default_pipeline.depthStencilState.Release();



	//Release
	//triangleVBuffer.Release();
	transformVBuffer.Release();
	//backBuffer.Release();
	for (unsigned int i = 0; i < default_pipeline.rendObjects.size(); ++i) {

		default_pipeline.rendObjects[i].indexBuffer->Release();
		default_pipeline.rendObjects[i].vertexBuffer->Release();

	}
	debugObjects.vertexBuffer->Release();

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

	//triangleVBuffer.p = nullptr;
	//backBuffer.p = nullptr;
	swapChain.p = nullptr;
	devContext.p = nullptr;
	device.p = nullptr;

}
