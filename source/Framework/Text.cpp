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
Text::Text(GfxDevice* gfxDevice, Font* pFont, Shader* pShader, XMFLOAT2 position, float scale) {
    m_GfxDevice = gfxDevice;
    m_Font = pFont;
    m_Shader = pShader;
    
    m_Position = position;
    m_Scale = scale;

    m_D3DVertexBuffer = NULL;

    m_XLength = 0.0f;
    m_TextAlign = TextAlign::ALIGN_LEFT;

    ////////// Rasterizer state //////////

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.AntialiasedLineEnable = TRUE;
    rasterizerDesc.MultisampleEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    m_GfxDevice->GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_D3DRasterizerState);

    constexpr int max_text_length = 65536;

    m_Text = (char*)malloc(max_text_length);
    SetText("Sample Text");
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
    strcpy(m_Text, new_text);

    ////////// Vertex Buffer //////////

    if (m_D3DVertexBuffer != NULL)
        m_D3DVertexBuffer->Release();

    size_t VertexDataSize = 4 * sizeof(Vertex) * strlen(m_Text);

    Vertex* VertexData = (Vertex*)malloc(4 * sizeof(Vertex) * strlen(m_Text));

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = VertexDataSize;
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    float x_cursor = 0.0f;
    float y_cursor = 0.0f;

    for (unsigned int i = 0; i < strlen(m_Text); i++) {
        Character* ch = m_Font->GetMappedChar(m_Text[i]);

        float xpos = x_cursor + ch->Bearing.x * m_Scale;
        float ypos = y_cursor - (ch->Size.y - ch->Bearing.y) * m_Scale;

        float w = ch->Size.x * m_Scale;
        float h = ch->Size.y * m_Scale;

        Vertex vertices[4] = {
            { { xpos,     ypos,     0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
            { { xpos,     ypos + h, 0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
            { { xpos + w, ypos,     0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
            { { xpos + w, ypos + h, 0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
        };

        memcpy(VertexData + (i * 4), vertices, sizeof(vertices));

        x_cursor += (ch->Advance >> 6) * m_Scale;
    }

    m_XLength = x_cursor;

    D3D11_SUBRESOURCE_DATA vertexData = { VertexData };

    m_GfxDevice->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_D3DVertexBuffer);

    free(VertexData);
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
    Transform scrTransform;
    switch (m_TextAlign) {
    case TextAlign::ALIGN_LEFT:
        scrTransform.Translate({ m_Position.x, m_Position.y, 0.0f });
        break;
    case TextAlign::ALIGN_CENTER:
        scrTransform.Translate({ m_Position.x - (m_XLength / 2), m_Position.y, 0.0f });
        break;
    case TextAlign::ALIGN_RIGHT:
        scrTransform.Translate({ m_Position.x - m_XLength, m_Position.y, 0.0f });
        break;
    }

    m_Constants.MVP = scrTransform.GetModelMatrix() * cam->GetOrthoMatrix();

    m_Shader->SetConstants(&m_Constants);
    m_Shader->Use();

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_GfxDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_GfxDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_D3DVertexBuffer, &stride, &offset);

    m_GfxDevice->GetDeviceContext()->RSSetState(m_D3DRasterizerState);

    for (unsigned int i = 0; i < strlen(m_Text); i++) {
        Character *ch = m_Font->GetMappedChar(m_Text[i]);

        ID3D11SamplerState* fontSamplerState = m_Font->GetSamplerState();

        m_GfxDevice->GetDeviceContext()->PSSetShaderResources(0, 1, &ch->textureView);
        m_GfxDevice->GetDeviceContext()->PSSetSamplers(0, 1, &fontSamplerState);

        m_GfxDevice->GetDeviceContext()->Draw(4, i*4);
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
    m_D3DVertexBuffer->Release();
    m_D3DRasterizerState->Release();

    free(m_Text);
}
