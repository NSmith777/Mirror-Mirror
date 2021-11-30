#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>

#include "GfxDevice.h"

using namespace DirectX;

typedef struct {
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
} Vertex;

class Model {
public:
	Model(GfxDevice* gfxDevice, const char *filepath);
	~Model();

	void Draw();

private:
	GfxDevice* m_GfxDevice;

	Vertex* VertexData;
	unsigned int VertexCount;

	ID3D11Buffer* vertexBuffer;
	ID3D11RasterizerState* rasterizerState;
};
