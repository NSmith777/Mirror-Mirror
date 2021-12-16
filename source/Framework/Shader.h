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

/*
GfxDevice relies on Camera, which relies on Shader,
so we need to declare it here to make it visible to the compiler!
*/
class GfxDevice;

class Shader {
public:
    Shader(GfxDevice* gfxDevice, const char* vs_path, const char* ps_path, unsigned int constantsSize);
    ~Shader();

    void Use();
    void SetConstants(void* pConstantsData);

    void BlendEnable(bool enable);
    void ZWriteEnable(bool enable);

private:
    std::vector<char> ReadData(const char* filename);

    GfxDevice* m_GfxDevice;

    ID3D11VertexShader* m_D3DVertexShader;
    ID3D11PixelShader* m_D3DPixelShader;
    ID3D11InputLayout* m_D3DInputLayout;

    ID3D11Buffer* m_D3DConstantBuffer;
    unsigned int m_ConstantsSize;

    D3D11_BLEND_DESC m_D3DBlendStateDesc;
    ID3D11BlendState* m_D3DBlendState;

    D3D11_DEPTH_STENCIL_DESC m_D3DDepthStencilDesc;
    ID3D11DepthStencilState* m_D3DDepthStencilState;
};
