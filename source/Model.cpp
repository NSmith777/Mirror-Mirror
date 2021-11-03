#include "Model.h"

Model::Model(GfxDevice* gfxDevice, const char* filepath) {
    m_GfxDevice = gfxDevice;

    FILE* mdl_file = fopen(filepath, "rb");

    if (!mdl_file)
        assert(false);

    int magic;
    fread(&magic, sizeof(int), 1, mdl_file);

    if (magic != 0x004C444D)
        assert(false);

    fread(&VertexCount, sizeof(unsigned int), 1, mdl_file);

    unsigned int VertexDataSize = VertexCount * sizeof(Vertex);
    VertexData = (Vertex*)malloc(VertexDataSize);
    fread(VertexData, VertexDataSize, 1, mdl_file);

    fclose(mdl_file);

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = VertexDataSize;
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = { VertexData };

    m_GfxDevice->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

    ////////// Model rasterizer state //////////

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    m_GfxDevice->GetDevice()->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
}

void Model::Draw() {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_GfxDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_GfxDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    m_GfxDevice->GetDeviceContext()->RSSetState(rasterizerState);

    m_GfxDevice->GetDeviceContext()->Draw(VertexCount, 0);
}

Model::~Model() {

}
