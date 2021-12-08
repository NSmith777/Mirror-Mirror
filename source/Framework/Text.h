//==============================================================================
// File: Text.h
// 
// Description: Declares the Text UI object.
// 
//==============================================================================

#pragma once

#include "GfxDevice.h"
#include "Model.h"
#include "Font.h"
#include "Camera.h"

#include <string>

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Text {
public:
    Text(GfxDevice* gfxDevice, Font* pFont, Shader* pShader, XMFLOAT2 pos, float new_scale);
    ~Text();

    const char* GetText() { return text; }
    void SetText(const char* new_text);

    void Render(Camera* cam);

    // Supress heap alignment warnings
    void* operator new(size_t i) { return _mm_malloc(i, 16); }
    void operator delete(void* p) { _mm_free(p); }

private:
    GfxDevice* m_GfxDevice;
    Font* m_Font;
    Shader* m_Shader;
    Constants constants;

    XMFLOAT2 position;
    float scale;

    Vertex VertexData[4];
    ID3D11Buffer* vertexBuffer;
    ID3D11RasterizerState* rasterizerState;

    char *text;
};
