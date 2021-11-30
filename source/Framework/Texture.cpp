#include "Texture.h"

Texture::Texture(GfxDevice *gfxDevice, const char *filepath) {
    m_GfxDevice = gfxDevice;
    textureView = NULL;

    ////////// Load BMP //////////

    FILE* bmp_file = fopen(filepath, "rb");

    if (!bmp_file)
        assert(false);

    BITMAPFILEHEADER bmp_header;
    fread(&bmp_header, sizeof(bmp_header), 1, bmp_file);

    BITMAPINFOHEADER bmp_info_header;
    fread(&bmp_info_header, sizeof(bmp_info_header), 1, bmp_file);

    width = *((int*)&bmp_info_header.biWidth);
    height = *((int*)&bmp_info_header.biHeight);
    bytes_per_pixel = *((short*)&bmp_info_header.biBitCount) / 8;

    pitch = width * bytes_per_pixel;

    char *pixel_data = new char[height * pitch];

    fseek(bmp_file, bmp_header.bfOffBits, SEEK_SET);
    fread(pixel_data, height * pitch, 1, bmp_file);

    fclose(bmp_file);

    ////////// Texture data //////////

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA textureData = {};
    textureData.pSysMem = pixel_data;
    textureData.SysMemPitch = pitch;

    m_GfxDevice->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);

    m_GfxDevice->GetDevice()->CreateShaderResourceView(texture, nullptr, &textureView);

    ////////// Sampler //////////

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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
