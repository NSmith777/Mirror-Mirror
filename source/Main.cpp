#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <fstream>
#include <vector>

#include "GfxDevice.h"
#include "Cube.h"

using namespace DirectX;

struct Constants
{
    XMMATRIX Transform;
    XMMATRIX Projection;
    XMFLOAT3 LightVector;
};

class Renderer {
public:
    Renderer(GfxDevice* gfxDevice);
    ~Renderer();

    void InitVertexBuffer();
    void InitShaders();
    void InitTexture();

    void Update(XMMATRIX composite_mtx);

private:
    GfxDevice* m_GfxDevice;

    ID3D11Buffer* vertexBuffer;

    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;

    ID3D11Buffer* constantBuffer;

    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* textureView;
};

Renderer::Renderer(GfxDevice* gfxDevice) {
    m_GfxDevice = gfxDevice;
}

Renderer::~Renderer() {

}

void Renderer::InitVertexBuffer() {
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(VertexData);
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = { VertexData };

    m_GfxDevice->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
}

std::vector<char> ReadData(const std::string filename) {
    std::vector<char> data;

    std::ifstream fs;
    fs.open(filename, std::ifstream::in | std::ifstream::binary);

    if (fs.good()) {
        fs.seekg(0, std::ios::end);
        size_t size = (size_t)fs.tellg();

        data.resize(size);
        fs.seekg(0, std::ios::beg);
        fs.read(data.data(), size);
        fs.close();
    }
    return data;
}

void Renderer::InitShaders() {
    std::vector<char> vertexShaderData = ReadData("../resources/shaders/default_vs.cso");
    std::vector<char> pixelShaderData = ReadData("../resources/shaders/default_ps.cso");

    m_GfxDevice->GetDevice()->CreateVertexShader(vertexShaderData.data(), vertexShaderData.size(), nullptr, &vertexShader);
    m_GfxDevice->GetDevice()->CreatePixelShader(pixelShaderData.data(), pixelShaderData.size(), nullptr, &pixelShader);

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = // float3 position, float2 texcoord, float3 normal
    {
        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    m_GfxDevice->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderData.data(), vertexShaderData.size(), &inputLayout);

    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = sizeof(Constants) + 0xf & 0xfffffff0; // round constant buffer size to 16 byte boundary
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    m_GfxDevice->GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
}

void Renderer::InitTexture() {
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
}

void Renderer::Update(XMMATRIX compositeMtx) {
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;

    m_GfxDevice->GetDeviceContext()->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

    Constants* constants = (Constants*)mappedSubresource.pData;

    constants->Transform = compositeMtx;
    constants->Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 16.0f / 9, 0.1f, 1000.0f);
    constants->LightVector = { 1.0f, -1.0f, 1.0f };

    m_GfxDevice->GetDeviceContext()->Unmap(constantBuffer, 0);

    ID3D11RenderTargetView* renderTarget = m_GfxDevice->GetRenderTarget();
    ID3D11DepthStencilView* depthStencil = m_GfxDevice->GetDepthStencil();

    ID3D11RasterizerState* rasterizerState = m_GfxDevice->GetRasterizerState();
    ID3D11SamplerState* samplerState = m_GfxDevice->GetSamplerState();
    ID3D11DepthStencilState* depthStencilState = m_GfxDevice->GetDepthStencilState();

    FLOAT clearColor[4] = { 0.25f, 0.25f, 0.25f, 1.0f };

    m_GfxDevice->GetDeviceContext()->ClearRenderTargetView(renderTarget, clearColor);
    m_GfxDevice->GetDeviceContext()->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_GfxDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_GfxDevice->GetDeviceContext()->IASetInputLayout(inputLayout);
    m_GfxDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    m_GfxDevice->GetDeviceContext()->VSSetShader(vertexShader, nullptr, 0);
    m_GfxDevice->GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer);

    m_GfxDevice->GetDeviceContext()->RSSetState(rasterizerState);

    m_GfxDevice->GetDeviceContext()->PSSetShader(pixelShader, nullptr, 0);
    m_GfxDevice->GetDeviceContext()->PSSetShaderResources(0, 1, &textureView);
    m_GfxDevice->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);

    m_GfxDevice->GetDeviceContext()->OMSetRenderTargets(1, &renderTarget, depthStencil);
    m_GfxDevice->GetDeviceContext()->OMSetDepthStencilState(depthStencilState, 0);
    m_GfxDevice->GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff); // use default blend mode (i.e. disable)

    m_GfxDevice->GetDeviceContext()->Draw(sizeof(VertexData) / sizeof(VertexData[0]), 0);
}

#define TITLE "Mirror, Mirror"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    WNDCLASSEXA wndClassEx = { sizeof(wndClassEx) };
    wndClassEx.lpfnWndProc = DefWindowProcA;
    wndClassEx.lpszClassName = TITLE;

    RegisterClassExA(&wndClassEx);

    HWND window = CreateWindowExA(NULL, TITLE, TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);

    ShowWindow(window, nCmdShow);

    GfxDevice myGfxDevice(window);
    myGfxDevice.InitD3DDevice();
    myGfxDevice.InitFrameBuffers();
    myGfxDevice.InitViewport();
    myGfxDevice.InitRenderStates();

    Renderer myRenderer(&myGfxDevice);
    myRenderer.InitVertexBuffer();
    myRenderer.InitShaders();
    myRenderer.InitTexture();

    XMFLOAT3 modelRotation = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 modelScale = { 1.0f, 1.0f, 1.0f };
    XMFLOAT3 modelTranslation = { 0.0f, 0.0f, 4.0f };

    while (true) {
        MSG msg;

        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_KEYDOWN) return 0;
            DispatchMessageA(&msg);
        }

        XMMATRIX scale = XMMatrixScaling(modelScale.x, modelScale.y, modelScale.z);
        XMMATRIX rotateX = XMMatrixRotationNormal({ 1, 0, 0 }, modelRotation.x);
        XMMATRIX rotateY = XMMatrixRotationNormal({ 0, 1, 0 }, modelRotation.y);
        XMMATRIX rotateZ = XMMatrixRotationNormal({ 0, 0, 1 }, modelRotation.z);
        XMMATRIX translate = XMMatrixTranslation(modelTranslation.x, modelTranslation.y, modelTranslation.z);

        modelRotation.x += 0.005f;
        modelRotation.y += 0.009f;
        modelRotation.z += 0.001f;

        XMMATRIX compositeMtx = scale * rotateX * rotateY * rotateZ * translate;

        myRenderer.Update(compositeMtx);

        myGfxDevice.Present();
    }
}
