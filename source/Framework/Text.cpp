//==============================================================================
// File: Text.cpp
// 
// Description: Implements the Text UI object.
// 
//==============================================================================

#include "Text.h"

//=============================================================================
// Text::Text
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	[GfxDevice *]   Graphics device object
//              [Font *]        Font object to use as this text's font
//              [Shader *]      Shader used to render this text to the screen
//              [XMFLOAT2]      Position in screen-space coordinates
//              [XMFLOAT2]      Text scaling in pixels
// 
// Return:      N/A
// 
//=============================================================================
Text::Text(GfxDevice* gfxDevice, Font* pFont, Shader* pShader, XMFLOAT2 new_pos, float new_scale) {
    m_GfxDevice = gfxDevice;
    m_Font = pFont;
    m_Shader = pShader;
    
    position = new_pos;
    scale = new_scale;

    constexpr int max_text_length = 65536;

    text = (char*)malloc(max_text_length);
    SetText("Sample Text");

    ////////// Vertex Buffer //////////

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(VertexData);
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA vertexData = { VertexData };

    m_GfxDevice->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

    ////////// Rasterizer state //////////

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.AntialiasedLineEnable = TRUE;
    rasterizerDesc.MultisampleEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    m_GfxDevice->GetDevice()->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
}

//=============================================================================
// Text::SetText
//=============================================================================
// 
// Description: Updates the text buffer with a new text string.
// 
// Parameters:	[const char *]  New text string to copy
// 
// Return:      N/A
// 
//=============================================================================
void Text::SetText(const char* new_text) {
    strcpy(text, new_text);
}

//=============================================================================
// Text::Render
//=============================================================================
// 
// Description: Draws this text to the screen, relative to a camera.
// 
// Parameters:	[Camera *]  Camera to draw relative to
// 
// Return:      N/A
// 
//=============================================================================
void Text::Render(Camera *cam) {
    constants.MVP = cam->GetOrthoMatrix();

    m_Shader->SetConstants(&constants);
    m_Shader->Use();

    float x_cursor = position.x;

    for (unsigned int i = 0; i < strlen(text); i++) {
        Character *ch = m_Font->GetMappedChar(text[i]);

        ID3D11SamplerState* fontSamplerState = m_Font->GetSamplerState();

        m_GfxDevice->GetDeviceContext()->PSSetShaderResources(0, 1, &ch->textureView);
        m_GfxDevice->GetDeviceContext()->PSSetSamplers(0, 1, &fontSamplerState);

        float xpos = x_cursor + ch->Bearing.x * scale;
        float ypos = position.y - (ch->Size.y - ch->Bearing.y) * scale;

        float w = ch->Size.x * scale;
        float h = ch->Size.y * scale;

        Vertex vertices[4] = {
            { { xpos,     ypos,     0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
            { { xpos,     ypos + h, 0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
            { { xpos + w, ypos,     0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
            { { xpos + w, ypos + h, 0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
        };

        D3D11_MAPPED_SUBRESOURCE resource;
        m_GfxDevice->GetDeviceContext()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
        memcpy(resource.pData, vertices, sizeof(vertices));
        m_GfxDevice->GetDeviceContext()->Unmap(vertexBuffer, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;

        m_GfxDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        m_GfxDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

        m_GfxDevice->GetDeviceContext()->RSSetState(rasterizerState);

        m_GfxDevice->GetDeviceContext()->Draw(4, 0);

        x_cursor += (ch->Advance >> 6) * scale;
    }
}

//=============================================================================
// Text::~Text
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Text::~Text() {
    vertexBuffer->Release();
    rasterizerState->Release();

    free(text);
}
