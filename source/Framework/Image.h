#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "GfxDevice.h"
#include "Model.h"
#include "Texture.h"
#include "Camera.h"

class Image {
public:
    Image(GfxDevice* gfxDevice, Texture* pTexture, Shader* pShader, XMFLOAT2 position, XMFLOAT2 size);
    ~Image();

    bool IsHovering(Camera* cam, XMINT2 mouse_pos);

    void Render(Camera* cam);

    // Supress heap alignment warnings
    void* operator new(size_t i) { return _mm_malloc(i, 16); }
    void operator delete(void* p) { _mm_free(p); }

protected:
    GfxDevice* m_GfxDevice;
    Texture* m_Texture;
    Shader* m_Shader;
    Constants m_Constants;

    XMFLOAT2 m_Position;
    XMFLOAT2 m_Size;

    ID3D11Buffer* m_D3DVertexBuffer;
    ID3D11RasterizerState* m_D3DRasterizerState;
};
