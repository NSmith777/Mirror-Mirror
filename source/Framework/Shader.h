//==============================================================================
// File: Shader.h
// 
// Description: Declares the Shader component.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

#include "GfxDevice.h"

using namespace DirectX;

typedef struct {
    XMMATRIX MVP;
    XMMATRIX MV;
    XMMATRIX M;
    XMMATRIX _World2Object;
    XMFLOAT3 _WorldSpaceCameraPos;
} Constants;

class GfxDevice;

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Shader {
public:
    Shader(GfxDevice* gfxDevice, const char* vs_path, const char* ps_path, unsigned int constantsSize);
    ~Shader();

    void Use();
    void SetConstants(void* pConstantsData);

    void BlendEnable(bool enable);
    void ZWriteEnable(bool enable);

private:
    GfxDevice* m_GfxDevice;

    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;

    ID3D11Buffer* constantBuffer;
    unsigned int m_constantsSize;

    D3D11_BLEND_DESC BlendStateDesc;
    ID3D11BlendState* blendState;

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ID3D11DepthStencilState* depthStencilState;

    std::vector<char> ReadData(const char* filename);
};
