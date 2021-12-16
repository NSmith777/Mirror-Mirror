#include "Model.h"

//=============================================================================
// Model::Model
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	[GfxDevice *]	Graphics device object
//              [const char *]  Path to the MDL file to load
// 
// Return:      N/A
// 
//=============================================================================
Model::Model(GfxDevice* gfxDevice, const char* filepath) {
    m_GfxDevice = gfxDevice;

    FILE* mdl_file = fopen(filepath, "rb");

    if (!mdl_file)
        assert(false);

    int magic;
    fread(&magic, sizeof(int), 1, mdl_file);

    if (magic != 0x004C444D)
        assert(false);

    fread(&m_VertexCount, sizeof(unsigned int), 1, mdl_file);

    unsigned int VertexDataSize = m_VertexCount * sizeof(Vertex);
    
    Vertex *VertexData = (Vertex*)malloc(VertexDataSize);

    fread(VertexData, VertexDataSize, 1, mdl_file);

    fclose(mdl_file);

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = VertexDataSize;
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = { VertexData };

    m_GfxDevice->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_D3DVertexBuffer);

    free(VertexData);

    ////////// Model rasterizer state //////////

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.AntialiasedLineEnable = TRUE;
    rasterizerDesc.MultisampleEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    m_GfxDevice->GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_D3DRasterizerState);
}

//=============================================================================
// Model::Draw
//=============================================================================
// 
// Description: Renders this 3D mesh to the screen.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Model::Draw() {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_GfxDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_GfxDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_D3DVertexBuffer, &stride, &offset);

    m_GfxDevice->GetDeviceContext()->RSSetState(m_D3DRasterizerState);

    m_GfxDevice->GetDeviceContext()->Draw(m_VertexCount, 0);
}

//=============================================================================
// Model::~Model
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Model::~Model() {
    m_D3DVertexBuffer->Release();
    m_D3DRasterizerState->Release();
}
