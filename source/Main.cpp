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
#include "Math.h"

using namespace DirectX;
using namespace Math;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    GfxDevice myGfxDevice;
    
    Shader myScreenShader(&myGfxDevice, "../resources/shaders/screen_vs.cso", "../resources/shaders/screen_ps.cso", sizeof(Constants));

    Camera myCamera(&myGfxDevice, &myScreenShader);
    myCamera.SetClearColor(0.25f, 0.25f, 0.25f);

    myCamera.GetTransform()->Translate(0.0f, 10.0f, -10.0f);
    myCamera.GetTransform()->Rotate(XMConvertToRadians(45.0f), 0.0f, 0.0f);

    Shader myShader(&myGfxDevice, "../resources/shaders/default_vs.cso", "../resources/shaders/default_ps.cso", sizeof(Constants));
    Texture myTexture(&myGfxDevice);

    Model PlayerMdl(&myGfxDevice, "../resources/models/TestPlayer.mdl");
    Transform PlayerTransform;

    Model DrawLineMdl(&myGfxDevice, "../resources/models/TestDrawLine.mdl");
    Transform DrawLineTransform;

    Model GroundMdl(&myGfxDevice, "../resources/models/CobbleGroundNormal.mdl");
    Transform GroundTransform;

    XMINT2 mouse_coords = { 0, 0 };
    XMFLOAT3 refl_point = { 0, 0, 0 };

    bool is_draw_line_visible = false;

    while (true) {
        ////////// Camera //////////

        myCamera.Use();

        XMFLOAT3 cam_offset = { 0.0f, 10.0f, -10.0f };

        XMFLOAT3 cam_lerp = XMFLOAT3_Lerp(myCamera.GetTransform()->GetPosition(), PlayerTransform.GetPosition() + cam_offset, 0.08f);

        myCamera.GetTransform()->SetPosition(cam_lerp.x, cam_lerp.y, cam_lerp.z);

        XMFLOAT3 cam_pos = myCamera.GetTransform()->GetPosition();
        XMFLOAT3 cam_ray_origin = myCamera.ScreenToWorldPoint(mouse_coords);

        // Raycast camera ray to the plane y=0
        XMFLOAT3 cam_ray_hit = -cam_pos.y * (cam_ray_origin - cam_pos) / (cam_ray_origin.y - cam_pos.y) + cam_pos;

        ////////// Input //////////

        MSG msg;
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            switch (msg.message) {
            case WM_KEYDOWN:
                return 0;
                break;
            case WM_MOUSEMOVE:
                mouse_coords.x = GET_X_LPARAM(msg.lParam);
                mouse_coords.y = GET_Y_LPARAM(msg.lParam);
                break;
            case WM_LBUTTONDOWN:
                DrawLineTransform.SetPosition(cam_ray_hit.x, 0.0f, cam_ray_hit.z);

                refl_point = cam_ray_hit;

                is_draw_line_visible = true;
                break;
            case WM_LBUTTONUP:
                XMFLOAT3 PlayerPos = PlayerTransform.GetPosition();

                float m = (cam_ray_hit.z - refl_point.z) / (cam_ray_hit.x - refl_point.x);
                float c = (cam_ray_hit.x * refl_point.z - refl_point.x * cam_ray_hit.z) / (cam_ray_hit.x - refl_point.x);
                float d = (PlayerPos.x + (PlayerPos.z - c) * m) / (1 + (m * m));

                float x4 = 2 * d - PlayerPos.x;
                float y4 = 2 * d * m - PlayerPos.z + 2 * c;

                PlayerTransform.SetPosition(x4, 0.0f, y4);

                is_draw_line_visible = false;
                break;
            }
            DispatchMessageA(&msg);
        }

        ////////// RENDERING //////////

        Constants constants1;
        constants1.MVP = PlayerTransform.GetModelMatrix() * myCamera.GetViewMatrix() * myCamera.GetProjMatrix();

        myShader.SetConstants(&constants1);

        myShader.Use();
        myTexture.Use();
        PlayerMdl.Draw();

        XMFLOAT3 draw_line_delta = cam_ray_hit - DrawLineTransform.GetPosition();

        DrawLineTransform.SetRotation(0, atan2f(draw_line_delta.x, draw_line_delta.z), 0);
        DrawLineTransform.SetScale(0.25f, 0.25f, XMFLOAT3_Distance(DrawLineTransform.GetPosition(), cam_ray_hit));

        if (is_draw_line_visible) {
            Constants constants2;
            constants2.MVP = DrawLineTransform.GetModelMatrix() * myCamera.GetViewMatrix() * myCamera.GetProjMatrix();

            myShader.SetConstants(&constants2);

            myShader.Use();
            myTexture.Use();
            DrawLineMdl.Draw();
        }

        Constants constants3;
        constants3.MVP = GroundTransform.GetModelMatrix() * myCamera.GetViewMatrix() * myCamera.GetProjMatrix();

        myShader.SetConstants(&constants3);

        myShader.Use();
        myTexture.Use();
        GroundMdl.Draw();

        myGfxDevice.Present(&myCamera, 1);
    }
}
