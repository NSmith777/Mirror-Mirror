#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "GfxDevice.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"

using namespace DirectX;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    GfxDevice myGfxDevice;

    Shader myScreenShader(&myGfxDevice, "../resources/shaders/screen_vs.cso", "../resources/shaders/screen_ps.cso", sizeof(Constants));

    Camera myCamera(&myGfxDevice, &myScreenShader);
    myCamera.SetClearColor(0.0f, 0.0f, 1.0f);

    myCamera.GetTransform()->Translate(0.0f, 10.0f, -10.0f);
    myCamera.GetTransform()->Rotate(XMConvertToRadians(45.0f), 0.0f, 0.0f);

    Shader myShader(&myGfxDevice, "../resources/shaders/default_vs.cso", "../resources/shaders/default_ps.cso", sizeof(Constants));
    Texture myTexture(&myGfxDevice);

    Model myModel1(&myGfxDevice, "../resources/models/TestPlayer.mdl");
    Transform myTransform1;
    myTransform1.Translate(0.0f, 0.0f, 0.0f);

    Model myModel2(&myGfxDevice, "../resources/models/TestPlatform.mdl");
    Transform myTransform2;
    myTransform2.Translate(0.0f, 0.0f, 0.0f);

    XMINT2 mouse2viewport = { 0, 0 };

    while (true) {
        MSG msg;

        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            switch (msg.message) {
            case WM_KEYDOWN:
                return 0;
                break;
            case WM_MOUSEMOVE:
                mouse2viewport.x = GET_X_LPARAM(msg.lParam);
                mouse2viewport.y = GET_Y_LPARAM(msg.lParam);
                break;
            }
            DispatchMessageA(&msg);
        }

        myCamera.Use();
        
        ////////// Model 1 //////////

        XMFLOAT3 cam_pos = myCamera.GetTransform()->GetPosition();
        XMFLOAT3 cam_ray_origin = myCamera.ScreenToWorldPoint(mouse2viewport);

        float player_x = -cam_pos.y * (cam_ray_origin.x - cam_pos.x) / (cam_ray_origin.y - cam_pos.y) + cam_pos.x;
        float player_z = -cam_pos.y * (cam_ray_origin.z - cam_pos.z) / (cam_ray_origin.y - cam_pos.y) + cam_pos.z;

        myTransform1.SetPosition(player_x, 0.0f, player_z);

        Constants constants1;
        constants1.MVP = myTransform1.GetModelMatrix() * myCamera.GetViewMatrix() * myCamera.GetProjMatrix();

        myShader.SetConstants(&constants1);

        myShader.Use();
        myTexture.Use();
        myModel1.Draw();

        ////////// Model 2 //////////

        Constants constants2;
        constants2.MVP = myTransform2.GetModelMatrix() * myCamera.GetViewMatrix() * myCamera.GetProjMatrix();

        myShader.SetConstants(&constants2);

        myShader.Use();
        myTexture.Use();
        myModel2.Draw();

        myGfxDevice.Present(&myCamera, 1);
    }
}
