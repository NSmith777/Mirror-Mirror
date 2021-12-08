//==============================================================================
// File: Image.cpp
// 
// Description: Implements the UI image object.
// 
//==============================================================================

#include "Image.h"

//=============================================================================
// Image::Image
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	[GfxDevice *]	Graphics device object
//              [Texture *]     Texture to use as this image
//              [Shader *]      Shader used to render this image to the screen
//              [XMFLOAT2]      Position in screen-space coordinates
//              [XMFLOAT2]      Image dimensions in pixels
// 
// Return:      N/A
// 
//=============================================================================
Image::Image(GfxDevice* gfxDevice, Texture* pTexture, Shader* pShader, XMFLOAT2 pos, XMFLOAT2 new_size) {
    m_GfxDevice = gfxDevice;
    m_Texture = pTexture;
    m_Shader = pShader;
    ZeroMemory(&constants, sizeof(constants));

    position = pos;
    size = new_size;

    ////////// Vertex Buffer //////////

    static const Vertex quadVerts[] = {
        { { 0.0f, 0.0f, 0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
        { { 0.0f, 1.0f, 0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
        { { 1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
        { { 1.0f, 1.0f, 0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
    };

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(quadVerts);
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = { quadVerts };

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
// Image::Render
//=============================================================================
// 
// Description: Draws this image to the screen, relative to a camera.
// 
// Parameters:	[Camera *]  Camera to draw relative to
// 
// Return:      N/A
// 
//=============================================================================
void Image::Render(Camera* cam) {
    Transform scrTransform;
    scrTransform.Translate({ position.x, position.y, 0.0f });
    scrTransform.SetScale({ size.x, size.y, 1.0f });

    constants.MVP = scrTransform.GetModelMatrix() * cam->GetOrthoMatrix();

    m_Shader->SetConstants(&constants);
    m_Shader->Use();

    m_Texture->Use();

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_GfxDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_GfxDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    m_GfxDevice->GetDeviceContext()->RSSetState(rasterizerState);

    m_GfxDevice->GetDeviceContext()->Draw(4, 0);
}

//=============================================================================
// Image::~Image
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Image::~Image() {
    vertexBuffer->Release();
    rasterizerState->Release();
}
