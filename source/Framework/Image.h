//==============================================================================
// File: Image.h
// 
// Description: Declares the UI image object.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "GfxDevice.h"
#include "Model.h"
#include "Texture.h"
#include "Camera.h"

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Image {
public:
    Image(GfxDevice* gfxDevice, Texture* pTexture, Shader* pShader, XMFLOAT2 pos, XMFLOAT2 new_size);
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
    Constants constants;

    XMFLOAT2 position;
    XMFLOAT2 size;

    ID3D11Buffer* vertexBuffer;
    ID3D11RasterizerState* rasterizerState;
};
