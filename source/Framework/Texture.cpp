#include "Texture.h"

//=============================================================================
// Texture::Texture
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	[GfxDevice *]	Graphics device object
//              [const char *]  Path to the bitmap image file
// 
// Return:      N/A
// 
//=============================================================================
Texture::Texture(GfxDevice *gfxDevice, const char *filepath) {
    m_GfxDevice = gfxDevice;
    m_D3DTextureView = NULL;

    ////////// Load BMP //////////

    FILE* bmp_file = fopen(filepath, "rb");

    if (!bmp_file)
        assert(false);

    BITMAPFILEHEADER bmp_header;
    fread(&bmp_header, sizeof(bmp_header), 1, bmp_file);

    BITMAPINFOHEADER bmp_info_header;
    fread(&bmp_info_header, sizeof(bmp_info_header), 1, bmp_file);

    m_Width = *((int*)&bmp_info_header.biWidth);
    m_Height = *((int*)&bmp_info_header.biHeight);
    m_BytesPerPixel = *((short*)&bmp_info_header.biBitCount) / 8;

    m_Pitch = m_Width * m_BytesPerPixel;

    m_PixelData = (char*)malloc(m_Height * m_Pitch);

    fseek(bmp_file, bmp_header.bfOffBits, SEEK_SET);
    fread(m_PixelData, m_Height * m_Pitch, 1, bmp_file);

    fclose(bmp_file);

    ////////// Texture data //////////

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = m_Width;
    textureDesc.Height = m_Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA textureData = {};
    textureData.pSysMem = m_PixelData;
    textureData.SysMemPitch = m_Pitch;

    m_GfxDevice->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &m_D3DTexture);

    m_GfxDevice->GetDevice()->CreateShaderResourceView(m_D3DTexture, nullptr, &m_D3DTextureView);

    ////////// Sampler //////////

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    m_GfxDevice->GetDevice()->CreateSamplerState(&samplerDesc, &m_D3DSamplerState);
}

//=============================================================================
// Texture::Use
//=============================================================================
// 
// Description: Binds this texture to the graphics backend.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Texture::Use() {
    m_GfxDevice->GetDeviceContext()->PSSetShaderResources(0, 1, &m_D3DTextureView);
    m_GfxDevice->GetDeviceContext()->PSSetSamplers(0, 1, &m_D3DSamplerState);
}

//=============================================================================
// Texture::~Texture
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Texture::~Texture() {
    m_D3DTexture->Release();
    m_D3DTextureView->Release();

    free(m_PixelData);

    m_D3DSamplerState->Release();
}
