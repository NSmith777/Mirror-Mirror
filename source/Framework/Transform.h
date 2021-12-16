#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "Math.h"

using namespace DirectX;
using namespace Math;

class Transform {
public:
    Transform();
    ~Transform();

    XMFLOAT3 GetPosition() { return m_Position; }
    XMFLOAT3 GetRotation() { return m_Rotation; }

    void SetPosition(XMFLOAT3 position) { m_Position = position; }
    void SetRotation(XMFLOAT3 rotation) { m_Rotation = rotation; }
    void SetScale(XMFLOAT3 scale) { m_Scale = scale; }

    void Translate(XMFLOAT3 translation) { m_Position = m_Position + translation; }
    void Rotate(XMFLOAT3 rotation) { m_Rotation = m_Rotation + rotation; }

    XMMATRIX GetModelMatrix();

    XMFLOAT3 GetRightVec();
    XMFLOAT3 GetUpVec();
    XMFLOAT3 GetForwardVec();

private:
    XMFLOAT3 m_Position;
    XMFLOAT3 m_Rotation;
    XMFLOAT3 m_Scale;
};
