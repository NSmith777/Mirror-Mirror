#include "Font.h"

Font::Font(GfxDevice* gfxDevice, FT_Library* pFt, const char *filepath, int size) {
    m_GfxDevice = gfxDevice;

    FT_Face face;
    if (FT_New_Face(*pFt, filepath, 0, &face))
        assert(false);

    FT_Set_Pixel_Sizes(face, 0, size);

    constexpr unsigned char max_ascii_chars = 128;

    for (unsigned char c = 0; c < max_ascii_chars; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = face->glyph->bitmap.width;
        textureDesc.Height = face->glyph->bitmap.rows;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA textureData = {};
        textureData.pSysMem = face->glyph->bitmap.buffer;
        textureData.SysMemPitch = face->glyph->bitmap.width;

        ID3D11Texture2D* texture;
        ID3D11ShaderResourceView* textureView;

        m_GfxDevice->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);
        m_GfxDevice->GetDevice()->CreateShaderResourceView(texture, nullptr, &textureView);

        Character character = {
            texture,
            textureView,

            XMINT2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            XMINT2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        Characters.insert(std::pair<char, Character>(c, character));
    }

    ////////// Sampler //////////

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    m_GfxDevice->GetDevice()->CreateSamplerState(&samplerDesc, &samplerState);
}

Font::~Font() {

}
