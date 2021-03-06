#pragma once

#include "Transform.h"

class BoxCollision {
public:
    BoxCollision(Transform* trans, XMFLOAT3 size);
    ~BoxCollision();

    bool Ray_Intersect(XMFLOAT3 L1, XMFLOAT3 L2, XMFLOAT3& Hit);

protected:
    void GetIntersection(float fDst1, float fDst2, XMFLOAT3 P1, XMFLOAT3 P2, XMFLOAT3& Hit);

    bool InBox(XMFLOAT3 Hit, XMFLOAT3 B1, XMFLOAT3 B2, const int Axis);

private:
    enum Axes { X, Y, Z };
    enum Planes { NX, NY, NZ, PX, PY, PZ, MAX_PLANES };

    typedef struct {
        XMFLOAT3 point;
        bool is_in_box;
    } Box_Hit;

    Transform* m_Transform;
    XMFLOAT3 m_BoxSize;
};
