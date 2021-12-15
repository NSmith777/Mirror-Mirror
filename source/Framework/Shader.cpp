//==============================================================================
// File: Shader.cpp
// 
// Description: Implements the Shader component.
// 
//==============================================================================

#include "Shader.h"

//=============================================================================
// Shader::Shader
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	[GfxDevice *]	Graphics device object
//              [const char *]  Path to the compiled vertex shader file
//              [const char *]  Path to the compiled pixel shader file
//              [unsigned int]  Size of this shader's constants structure
// 
// Return:      N/A
// 
//=============================================================================
Shader::Shader(GfxDevice* gfxDevice, const char *vs_path, const char *ps_path, unsigned int constantsSize) {
    m_GfxDevice = gfxDevice;
    m_ConstantsSize = constantsSize;

    std::vector<char> vertexShaderData = ReadData(vs_path);
    std::vector<char> pixelShaderData = ReadData(ps_path);

    m_GfxDevice->GetDevice()->CreateVertexShader(vertexShaderData.data(), vertexShaderData.size(), nullptr, &m_D3DVertexShader);
    m_GfxDevice->GetDevice()->CreatePixelShader(pixelShaderData.data(), pixelShaderData.size(), nullptr, &m_D3DPixelShader);

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = 
    {
        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    m_GfxDevice->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderData.data(), vertexShaderData.size(), &m_D3DInputLayout);

    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = m_ConstantsSize + 0xf & 0xfffffff0; // round constant buffer size to 16 byte boundary
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    m_GfxDevice->GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &m_D3DConstantBuffer);

    ////////// Blend State //////////

    m_D3DBlendStateDesc.AlphaToCoverageEnable = FALSE;
    m_D3DBlendStateDesc.IndependentBlendEnable = FALSE;
    m_D3DBlendStateDesc.RenderTarget[0].BlendEnable = FALSE;
    m_D3DBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    m_D3DBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    m_D3DBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    m_D3DBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    m_D3DBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    m_D3DBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    m_D3DBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    m_GfxDevice->GetDevice()->CreateBlendState(&m_D3DBlendStateDesc, &m_D3DBlendState);

    ////////// Depth Stencil State //////////

    m_D3DDepthStencilDesc.DepthEnable = TRUE;
    m_D3DDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_D3DDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    m_GfxDevice->GetDevice()->CreateDepthStencilState(&m_D3DDepthStencilDesc, &m_D3DDepthStencilState);
}

//=============================================================================
// Shader::ReadData
//=============================================================================
// 
// Description: Reads the entirety of a specified file into an output buffer.
// 
// Parameters:	[const char *]      Path to the file to read
// 
// Return:      [std::vector<char>] Output buffer containing the file data.
//                                  If file failed to open, returns an empty vector.
// 
//=============================================================================
std::vector<char> Shader::ReadData(const char* filename) {
    std::vector<char> data;

    FILE* fs = fopen(filename, "rb");

    if (!fs)
        assert(false);

    fseek(fs, 0, SEEK_END);

    size_t size = ftell(fs);

    data.resize(size);
    fseek(fs, 0, SEEK_SET);
    fread(data.data(), size, 1, fs);
    fclose(fs);

    return data;
}

//=============================================================================
// Shader::Use
//=============================================================================
// 
// Description: Binds this shader to the graphics backend.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Shader::Use() {
    m_GfxDevice->GetDeviceContext()->IASetInputLayout(m_D3DInputLayout);
    m_GfxDevice->GetDeviceContext()->VSSetShader(m_D3DVertexShader, nullptr, 0);
    m_GfxDevice->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_D3DConstantBuffer);

    m_GfxDevice->GetDeviceContext()->PSSetShader(m_D3DPixelShader, nullptr, 0);

    m_GfxDevice->GetDeviceContext()->OMSetBlendState(m_D3DBlendState, nullptr, 0xffffffff);
    m_GfxDevice->GetDeviceContext()->OMSetDepthStencilState(m_D3DDepthStencilState, 0);
}

//=============================================================================
// Shader::SetConstants
//=============================================================================
// 
// Description: Updates the constants data in this shader's constant buffer.
// 
// Parameters:	[void *]    Constants structure to upload
// 
// Return:      N/A
// 
//=============================================================================
void Shader::SetConstants(void *pConstantsData) {
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;

    m_GfxDevice->GetDeviceContext()->Map(m_D3DConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

    memcpy(mappedSubresource.pData, pConstantsData, m_ConstantsSize);

    m_GfxDevice->GetDeviceContext()->Unmap(m_D3DConstantBuffer, 0);
}

//=============================================================================
// Shader::BlendEnable
//=============================================================================
// 
// Description: Controls colour/alpha blending enable on the graphics backend.
// 
// Parameters:	[bool]  Enable blending?
// 
// Return:      N/A
// 
//=============================================================================
void Shader::BlendEnable(bool enable) {
    m_D3DBlendStateDesc.AlphaToCoverageEnable = enable;
    m_D3DBlendStateDesc.IndependentBlendEnable = enable;
    m_D3DBlendStateDesc.RenderTarget[0].BlendEnable = enable;

    // Update blend state resource
    m_D3DBlendState->Release();
    m_GfxDevice->GetDevice()->CreateBlendState(&m_D3DBlendStateDesc, &m_D3DBlendState);
}

//=============================================================================
// Shader::ZWriteEnable
//=============================================================================
// 
// Description: Controls depth buffer write enable on the graphics backend.
// 
// Parameters:	[bool]  Enable depth write?
// 
// Return:      N/A
// 
//=============================================================================
void Shader::ZWriteEnable(bool enable) {
    m_D3DDepthStencilDesc.DepthEnable = enable;

    // Update depth-stencil state resource
    m_D3DDepthStencilState->Release();
    m_GfxDevice->GetDevice()->CreateDepthStencilState(&m_D3DDepthStencilDesc, &m_D3DDepthStencilState);
}

//=============================================================================
// Shader::~Shader
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Shader::~Shader() {
    m_D3DVertexShader->Release();
    m_D3DPixelShader->Release();
    m_D3DInputLayout->Release();

    m_D3DConstantBuffer->Release();

    m_D3DBlendState->Release();

    m_D3DDepthStencilState->Release();
}
