#pragma once

#include <windows.h>
#include <d3d11.h>
#include <vector>

#include "GfxDevice.h"

class GfxDevice;

class Shader {
public:
    Shader(GfxDevice* gfxDevice, const char* vs_path, const char* ps_path, unsigned int constantsSize);
    ~Shader();

    void Use();
    void SetConstants(void* pConstantsData);

private:
    GfxDevice* m_GfxDevice;

    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;

    ID3D11Buffer* constantBuffer;
    unsigned int m_constantsSize;
};
