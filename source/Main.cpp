#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Framework/GfxDevice.h"
#include "Framework/Model.h"
#include "Framework/Texture.h"
#include "Framework/Shader.h"
#include "Framework/Transform.h"
#include "Framework/Camera.h"
#include "Framework/Collision.h"
#include "Framework/Math.h"

#include "GameObject.h"

using namespace DirectX;
using namespace Math;

bool ccw(XMFLOAT3 A, XMFLOAT3 B, XMFLOAT3 C) {
    return (C.z - A.z) * (B.x - A.x) > (B.z - A.z) * (C.x - A.x);
}

bool is_xz_line_intersect(XMFLOAT3 A, XMFLOAT3 B, XMFLOAT3 C, XMFLOAT3 D) {
    return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    GfxDevice myGfxDevice;
    
    Shader myScreenShader(&myGfxDevice, "../resources/shaders/screen_vs.cso", "../resources/shaders/screen_ps.cso", sizeof(Constants));
    Shader myShader(&myGfxDevice, "../resources/shaders/default_vs.cso", "../resources/shaders/default_ps.cso", sizeof(Constants));
    myShader.BlendEnable(true);

    Texture PlayerTex(&myGfxDevice, "../resources/objects/Player/TestTexture.bmp");
    Texture DrawLineGuideTex(&myGfxDevice, "../resources/objects/DrawLine/DrawLine_Guide_Dif.bmp");
    Texture DrawLineTargetTex(&myGfxDevice, "../resources/objects/DrawLine/DrawLine_Target_Dif.bmp");
    Texture DrawLineTargetNGTex(&myGfxDevice, "../resources/objects/DrawLine/DrawLine_Target_NG_Dif.bmp");
    Texture GroundTex(&myGfxDevice, "../resources/objects/CobbleGroundNormal/CobbleGroundNormal_Dif.bmp");
    Texture WallTex(&myGfxDevice, "../resources/objects/StoneWall/StoneWall_Dif.bmp");

    Model PlayerMdl(&myGfxDevice, "../resources/objects/Player/TestPlayer.mdl");
    Model DrawLineGuideMdl(&myGfxDevice, "../resources/objects/DrawLine/TestDrawLine_Guide.mdl");
    Model DrawLineTargetMdl(&myGfxDevice, "../resources/objects/DrawLine/TestDrawLine_Target.mdl");
    Model GroundMdl(&myGfxDevice, "../resources/objects/CobbleGroundNormal/CobbleGroundNormal.mdl");
    Model WallMdl(&myGfxDevice, "../resources/objects/StoneWall/StoneWall.mdl");

    GameObject Player(&PlayerMdl, &PlayerTex, &myShader);
    GameObject MirrorLine(&DrawLineGuideMdl, &DrawLineGuideTex, &myShader);
    GameObject ReflectLine(&DrawLineGuideMdl, &DrawLineGuideTex, &myShader);
    GameObject DrawLineTarget(&DrawLineTargetMdl, &DrawLineTargetTex, &myShader);
    GameObject Ground(&GroundMdl, &GroundTex, &myShader);
    GameObject Wall(&WallMdl, &WallTex, &myShader);

    BoxCollision WallCollider(Wall.GetTransform(), { 4, 8, 4 });

    DrawLineTarget.GetTransform()->SetScale(1.75f, 1.75f, 1.75f);
    Wall.GetTransform()->SetPosition(-8.0f, 0.0f, 0.0f);

    Camera myCamera(&myGfxDevice, &myScreenShader);
    myCamera.SetClearColor(0.25f, 0.25f, 0.25f);

    myCamera.GetTransform()->SetPosition(0.0f, 20.0f, -20.0f);
    myCamera.GetTransform()->SetRotation(XMConvertToRadians(45.0f), 0.0f, 0.0f);

    XMINT2 mouse_coords = { 0, 0 };
    XMFLOAT3 mirror_start = { 0, 0, 0 };
    XMFLOAT3 mirror_target = { 0, 0, 0 };

    bool is_mouse_held = false;
    bool is_reflect_line_drawn = false;
    bool can_move = false;

    while (true) {
        XMFLOAT3 PlayerPos = Player.GetTransform()->GetPosition();
        Transform* MirrorLineTransform = MirrorLine.GetTransform();
        Transform* ReflectLineTransform = ReflectLine.GetTransform();

        ////////// Camera //////////

        myCamera.Use();

        XMFLOAT3 cam_offset = { 0.0f, 20.0f, -20.0f };

        XMFLOAT3 cam_lerp = XMFLOAT3_Lerp(myCamera.GetTransform()->GetPosition(), PlayerPos + cam_offset, 0.08f);

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
                MirrorLineTransform->SetPosition(cam_ray_hit.x, cam_ray_hit.y, cam_ray_hit.z);
                ReflectLineTransform->SetPosition(PlayerPos.x, PlayerPos.y, PlayerPos.z);

                mirror_start = cam_ray_hit;

                is_mouse_held = true;
                break;
            case WM_LBUTTONUP:
                if(is_reflect_line_drawn && can_move)
                    Player.GetTransform()->SetPosition(mirror_target.x, mirror_target.y, mirror_target.z);

                is_mouse_held = false;
                break;
            }
            DispatchMessageA(&msg);
        }

        ////////// CALCULATE DRAW LINES //////////

        if (is_mouse_held) {
            float m = (cam_ray_hit.z - mirror_start.z) / (cam_ray_hit.x - mirror_start.x);
            float c = (cam_ray_hit.x * mirror_start.z - mirror_start.x * cam_ray_hit.z) / (cam_ray_hit.x - mirror_start.x);
            float d = (PlayerPos.x + (PlayerPos.z - c) * m) / (1 + (m * m));

            mirror_target.x = 2 * d - PlayerPos.x;
            mirror_target.z = 2 * d * m - PlayerPos.z + 2 * c;

            is_reflect_line_drawn = is_xz_line_intersect(
                MirrorLineTransform->GetPosition(),
                cam_ray_hit,
                ReflectLineTransform->GetPosition(),
                mirror_target
            );

            // --------------------

            XMFLOAT3 mirror_line_delta = cam_ray_hit - MirrorLineTransform->GetPosition();

            MirrorLineTransform->SetRotation(0, atan2f(mirror_line_delta.x, mirror_line_delta.z), 0);
            MirrorLineTransform->SetScale(0.4f, 0.4f, XMFLOAT3_Distance(MirrorLineTransform->GetPosition(), cam_ray_hit));

            // --------------------

            XMFLOAT3 reflect_line_delta = mirror_target - ReflectLineTransform->GetPosition();

            ReflectLineTransform->SetRotation(0, atan2f(reflect_line_delta.x, reflect_line_delta.z), 0);
            ReflectLineTransform->SetScale(0.4f, 0.4f, XMFLOAT3_Distance(ReflectLineTransform->GetPosition(), mirror_target));

            // --------------------

            XMFLOAT3 out_hit;

            can_move = WallCollider.Ray_Intersect(
                ReflectLineTransform->GetPosition(),
                mirror_target,
                out_hit
            ) ? false : true;

            if (can_move) {
                DrawLineTarget.GetTransform()->SetPosition(mirror_target.x, mirror_target.y, mirror_target.z);
                DrawLineTarget.GetTransform()->Rotate(0.0f, XMConvertToRadians(1.5f), 0.0f);
            }
            else {
                DrawLineTarget.GetTransform()->SetPosition(out_hit.x, out_hit.y, out_hit.z);
                DrawLineTarget.GetTransform()->SetRotation(0.0f, 0.0f, 0.0f);
            }
        }

        ////////// RENDERING //////////

        Player.Render(&myCamera);
        Ground.Render(&myCamera);
        Wall.Render(&myCamera);

        if (is_mouse_held) {
            // Disable depth write for all lines
            myShader.ZWriteEnable(false);

            MirrorLine.Render(&myCamera);

            if (is_reflect_line_drawn) {
                ReflectLine.Render(&myCamera);

                if (can_move)
                    DrawLineTarget.SetTexture(&DrawLineTargetTex);
                else
                    DrawLineTarget.SetTexture(&DrawLineTargetNGTex);

                DrawLineTarget.Render(&myCamera);
            }

            // Re-enable depth write
            myShader.ZWriteEnable(true);
        }

        myGfxDevice.Present(&myCamera, 1);
    }
}
