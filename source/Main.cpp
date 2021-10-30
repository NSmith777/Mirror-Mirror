#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "GfxDevice.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

using namespace DirectX;

typedef struct {
    XMMATRIX Transform;
    XMMATRIX Projection;
    XMFLOAT3 LightVector;
} Constants;

#define TITLE "Mirror, Mirror"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    WNDCLASSEXA wndClassEx = { sizeof(wndClassEx) };
    wndClassEx.lpfnWndProc = DefWindowProcA;
    wndClassEx.lpszClassName = TITLE;

    RegisterClassExA(&wndClassEx);

    HWND window = CreateWindowExA(NULL, TITLE, TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);

    ShowWindow(window, nCmdShow);

    GfxDevice myGfxDevice(window);

    Shader myShader(&myGfxDevice, "../resources/shaders/default_vs.cso", "../resources/shaders/default_ps.cso", sizeof(Constants));
    Texture myTexture(&myGfxDevice);
    Model myModel(&myGfxDevice, "../resources/models/test.mdl");

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

        // Set constants here...
        Constants constants;
        constants.Transform = compositeMtx;
        constants.Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 16.0f / 9, 0.1f, 1000.0f);
        constants.LightVector = { 1.0f, -1.0f, 1.0f };

        myShader.SetConstants(&constants);

        ID3D11RenderTargetView* renderTarget = myGfxDevice.GetRenderTarget();
        ID3D11DepthStencilView* depthStencil = myGfxDevice.GetDepthStencil();

        ID3D11RasterizerState* rasterizerState = myGfxDevice.GetRasterizerState();
        ID3D11SamplerState* samplerState = myGfxDevice.GetSamplerState();
        ID3D11DepthStencilState* depthStencilState = myGfxDevice.GetDepthStencilState();

        FLOAT clearColor[4] = { 0.25f, 0.25f, 0.25f, 1.0f };

        myGfxDevice.GetDeviceContext()->ClearRenderTargetView(renderTarget, clearColor);
        myGfxDevice.GetDeviceContext()->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

        myGfxDevice.GetDeviceContext()->RSSetState(rasterizerState);

        myGfxDevice.GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);

        myGfxDevice.GetDeviceContext()->OMSetRenderTargets(1, &renderTarget, depthStencil);
        myGfxDevice.GetDeviceContext()->OMSetDepthStencilState(depthStencilState, 0);
        myGfxDevice.GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff); // use default blend mode (i.e. disable)

        myShader.Use();
        myTexture.Use();
        myModel.Draw();

        myGfxDevice.Present();
    }
}
