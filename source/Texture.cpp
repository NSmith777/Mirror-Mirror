#include "Texture.h"

#define TEXTURE_WIDTH  8
#define TEXTURE_HEIGHT 8

UINT TextureData[] =
{
    0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f,
    0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff,
    0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f,
    0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff,
    0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f,
    0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff,
    0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f,
    0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff,
};

Texture::Texture(GfxDevice *gfxDevice) {
    m_GfxDevice = gfxDevice;
    textureView = NULL;

    ////////// Texture data //////////

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = TEXTURE_WIDTH;
    textureDesc.Height = TEXTURE_HEIGHT;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA textureData = {};
    textureData.pSysMem = TextureData;
    textureData.SysMemPitch = TEXTURE_WIDTH * 4; // 4 bytes per pixel

    m_GfxDevice->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);

    m_GfxDevice->GetDevice()->CreateShaderResourceView(texture, nullptr, &textureView);

    ////////// Sampler //////////

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    m_GfxDevice->GetDevice()->CreateSamplerState(&samplerDesc, &samplerState);
}

void Texture::Use() {
    m_GfxDevice->GetDeviceContext()->PSSetShaderResources(0, 1, &textureView);
    m_GfxDevice->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
}

Texture::~Texture() {

}
