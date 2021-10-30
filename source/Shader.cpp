#include "Shader.h"

std::vector<char> ReadData(const char *filename) {
    std::vector<char> data;

    FILE* fs = fopen(filename, "rb");

    if (fs) {
        fseek(fs, 0, SEEK_END);

        size_t size = ftell(fs);

        data.resize(size);
        fseek(fs, 0, SEEK_SET);
        fread(data.data(), size, 1, fs);
        fclose(fs);
    }
    return data;
}

Shader::Shader(GfxDevice* gfxDevice, const char *vs_path, const char *ps_path, unsigned int constantsSize) {
    m_GfxDevice = gfxDevice;
    m_constantsSize = constantsSize;

    std::vector<char> vertexShaderData = ReadData(vs_path);
    std::vector<char> pixelShaderData = ReadData(ps_path);

    m_GfxDevice->GetDevice()->CreateVertexShader(vertexShaderData.data(), vertexShaderData.size(), nullptr, &vertexShader);
    m_GfxDevice->GetDevice()->CreatePixelShader(pixelShaderData.data(), pixelShaderData.size(), nullptr, &pixelShader);

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = 
    {
        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    m_GfxDevice->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderData.data(), vertexShaderData.size(), &inputLayout);

    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = m_constantsSize + 0xf & 0xfffffff0; // round constant buffer size to 16 byte boundary
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    m_GfxDevice->GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
}

void Shader::Use() {
    m_GfxDevice->GetDeviceContext()->IASetInputLayout(inputLayout);
    m_GfxDevice->GetDeviceContext()->VSSetShader(vertexShader, nullptr, 0);
    m_GfxDevice->GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer);

    m_GfxDevice->GetDeviceContext()->PSSetShader(pixelShader, nullptr, 0);
}

void Shader::SetConstants(void *pConstantsData) {
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;

    m_GfxDevice->GetDeviceContext()->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

    memcpy(mappedSubresource.pData, pConstantsData, m_constantsSize);

    m_GfxDevice->GetDeviceContext()->Unmap(constantBuffer, 0);
}

Shader::~Shader() {

}
