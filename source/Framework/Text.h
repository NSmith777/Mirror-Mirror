#pragma once

#include "GfxDevice.h"
#include "Model.h"
#include "Font.h"
#include "Camera.h"

#include <string>

class Text {
public:
    Text(GfxDevice* gfxDevice, Font* pFont, Shader* pShader, XMFLOAT2 position, float scale);
    ~Text();

    const char* GetText() { return m_Text; }
    void SetText(const char* new_text);

    void SetPosition(XMFLOAT2 position) { m_Position = position; }

    void Render(Camera* cam);

    enum class TextAlign {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT,
    };

    void SetJustify(TextAlign align) { m_TextAlign = align; }

    // Supress heap alignment warnings
    void* operator new(size_t i) { return _mm_malloc(i, 16); }
    void operator delete(void* p) { _mm_free(p); }

private:
    GfxDevice* m_GfxDevice;
    Font* m_Font;
    Shader* m_Shader;
    Constants m_Constants;

    XMFLOAT2 m_Position;
    float m_Scale;

    ID3D11Buffer* m_D3DVertexBuffer;
    ID3D11RasterizerState* m_D3DRasterizerState;

    float m_XLength;
    TextAlign m_TextAlign;

    char *m_Text;
};
