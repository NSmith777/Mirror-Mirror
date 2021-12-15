//==============================================================================
// File: Model.h
// 
// Description: Declares the Model (3D mesh) component.
// 
//==============================================================================

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

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Model {
public:
    Model(GfxDevice* gfxDevice, const char *filepath);
    ~Model();

    void Draw();

private:
    GfxDevice* m_GfxDevice;

    unsigned int m_VertexCount;

    ID3D11Buffer* m_D3DVertexBuffer;
    ID3D11RasterizerState* m_D3DRasterizerState;
};
