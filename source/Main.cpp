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
#include "Framework/Font.h"
#include "Framework/Text.h"
#include "Framework/Image.h"

#include "GameObject.h"

using namespace DirectX;
using namespace Math;

static inline bool ccw(XMFLOAT3 A, XMFLOAT3 B, XMFLOAT3 C) {
    return (C.z - A.z) * (B.x - A.x) > (B.z - A.z) * (C.x - A.x);
}

static inline bool is_xz_line_intersect(XMFLOAT3 A, XMFLOAT3 B, XMFLOAT3 C, XMFLOAT3 D) {
    return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    GfxDevice myGfxDevice;
    
    Shader myScreenShader(&myGfxDevice, "../resources/shaders/screen_vs.cso", "../resources/shaders/screen_ps.cso", sizeof(Constants));
    Shader myTextShader(&myGfxDevice, "../resources/shaders/text_vs.cso", "../resources/shaders/text_ps.cso", sizeof(Constants));
    Shader myShader(&myGfxDevice, "../resources/shaders/default_vs.cso", "../resources/shaders/default_ps.cso", sizeof(Constants));

    myTextShader.ZWriteEnable(false);
    myTextShader.BlendEnable(true);

    myShader.BlendEnable(true);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        assert(false);

    Font myFont(&myGfxDevice, &ft, "../resources/fonts/Roboto-Medium.ttf", 54);

    Text myText1(&myGfxDevice, &myFont, &myTextShader, { -620, 300 }, 0.5f);
    Text myText2(&myGfxDevice, &myFont, &myTextShader, { -620, 200 }, 1.0f);

    myText1.SetText("Test Text 1");
    myText2.SetText("Sample Text 2");

    Texture PlayerTex(&myGfxDevice, "../resources/objects/Player/TestTexture.bmp");
    Texture DrawLineGuideTex(&myGfxDevice, "../resources/objects/DrawLine/DrawLine_Guide_Dif.bmp");
    Texture DrawLineTargetTex(&myGfxDevice, "../resources/objects/DrawLine/DrawLine_Target_Dif.bmp");
    Texture DrawLineTargetNGTex(&myGfxDevice, "../resources/objects/DrawLine/DrawLine_Target_NG_Dif.bmp");
    Texture GroundTex(&myGfxDevice, "../resources/objects/CobbleGroundNormal/CobbleGroundNormal_Dif.bmp");
    Texture WallTex(&myGfxDevice, "../resources/objects/StoneWall/StoneWall_Dif.bmp");

    Image myImage(&myGfxDevice, &WallTex, &myShader, { -620, -300 }, { 200, 200 });

    Model PlayerMdl(&myGfxDevice, "../resources/objects/Player/TestPlayer.mdl");
    Model DrawLineGuideMdl(&myGfxDevice, "../resources/objects/DrawLine/TestDrawLine_Guide.mdl");
    Model DrawLineTargetMdl(&myGfxDevice, "../resources/objects/DrawLine/TestDrawLine_Target.mdl");
    Model GroundMdl(&myGfxDevice, "../resources/objects/CobbleGroundNormal/CobbleGroundNormal.mdl");
    Model WallMdl(&myGfxDevice, "../resources/objects/StoneWall/StoneWall.mdl");

    GameObject Player(&PlayerMdl, &PlayerTex, &myShader);
    GameObject MirrorLine(&DrawLineGuideMdl, &DrawLineGuideTex, &myShader);
    GameObject ReflectLine(&DrawLineGuideMdl, &DrawLineGuideTex, &myShader);
    GameObject DrawLineTarget(&DrawLineTargetMdl, &DrawLineTargetTex, &myShader);

    std::vector<GameObject> Grounds;
    std::vector<GameObject> Walls;

    FILE* lvlfile = fopen("../resources/levels/lvl0.txt", "r");
    char lvlfile_buf[512] = {'\0'};

    while (fscanf(lvlfile, "%s\n", lvlfile_buf) == 1) {
        if (strcmp(lvlfile_buf, "LAYER") == 0) {
            int width = 0, height = 0;
            if (fscanf(lvlfile, "%i %i\n", &width, &height) != 2)
                assert(false);

            for (int z = 0; z < height; z++) {
                for (int x = 0; x < width; x++) {
                    int gameobject_id = 0;
                    if (fscanf(lvlfile, "%i,", &gameobject_id) != 1)
                        assert(false);

                    XMFLOAT3 obj_pos(x * 8.0f, 0.0f, z * -8.0f);

                    switch (gameobject_id) {
                    case 1:
                        Player.GetTransform()->SetPosition(obj_pos.x, obj_pos.y, obj_pos.z);
                        break;
                    case 2:
                        break;
                    case 3:
                        Grounds.push_back(GameObject(&GroundMdl, &GroundTex, &myShader));

                        Grounds[Grounds.size() - 1].AddBoxCollider({ 4, 4, 4 });
                        Grounds[Grounds.size() - 1].GetTransform()->SetPosition(obj_pos.x, obj_pos.y, obj_pos.z);
                        break;
                    case 4:
                        Walls.push_back(GameObject(&WallMdl, &WallTex, &myShader));

                        Walls[Walls.size() - 1].AddBoxCollider({ 4, 8, 4 });
                        Walls[Walls.size() - 1].GetTransform()->SetPosition(obj_pos.x, obj_pos.y, obj_pos.z);
                        break;
                    default:
                        break;
                    }
                }

                if (fscanf(lvlfile, "\n") != 0)
                    assert(false);
            }
        }
    }

    DrawLineTarget.GetTransform()->SetScale(1.75f, 1.75f, 1.75f);

    Camera myCamera(&myGfxDevice, &myScreenShader);
    myCamera.SetClearColor(0.25f, 0.25f, 0.25f);

    myCamera.GetTransform()->SetPosition(0.0f, 20.0f, -20.0f);
    myCamera.GetTransform()->SetRotation(XMConvertToRadians(60.0f), 0.0f, 0.0f);

    XMINT2 mouse_coords(0, 0);
    XMFLOAT3 mirror_start(0, 0, 0);
    XMFLOAT3 mirror_target(0, 0, 0);

    bool is_mouse_held = false;
    bool is_reflect_line_drawn = false;
    bool can_move = false;

    while (true) {
        XMFLOAT3 PlayerPos = Player.GetTransform()->GetPosition();
        Transform* MirrorLineTransform = MirrorLine.GetTransform();
        Transform* ReflectLineTransform = ReflectLine.GetTransform();

        ////////// Camera //////////

        myCamera.Use();

        XMFLOAT3 cam_offset(0.0f, 30.0f, -18.0f);

        XMFLOAT3 cam_lerp = XMFLOAT3_Lerp(myCamera.GetTransform()->GetPosition(), PlayerPos + cam_offset, 0.1f);

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

            std::vector<XMFLOAT3> hits;

            for (unsigned int i = 0; i < Walls.size(); i++) {
                XMFLOAT3 cur_hit;

                bool is_hit = Walls[i].GetBoxCollision()->Ray_Intersect(
                    ReflectLineTransform->GetPosition(),
                    mirror_target,
                    cur_hit
                );

                if (is_hit)
                    hits.push_back(cur_hit);
            }

            XMFLOAT3 out_hit;

            if (hits.size() > 0) {
                float distance = XMFLOAT3_Distance(ReflectLineTransform->GetPosition(), hits[0]);
                int closest_hit = 0;

                for (unsigned int i = 0; i < hits.size(); i++) {
                    if (XMFLOAT3_Distance(ReflectLineTransform->GetPosition(), hits[i]) < distance)
                        closest_hit = i;
                }

                out_hit = hits[closest_hit];
                can_move = false;
            }
            else {
                can_move = true;
            }

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

        for (unsigned int i = 0; i < Grounds.size(); i++)
            Grounds[i].Render(&myCamera);
        
        for (unsigned int i = 0; i < Walls.size(); i++)
            Walls[i].Render(&myCamera);

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

        // Draw text
        myText1.Render(&myCamera);
        myText2.Render(&myCamera);

        myImage.Render(&myCamera);

        myGfxDevice.Present(&myCamera, 1);
    }
}
