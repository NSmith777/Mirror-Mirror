#include "Collision.h"
#include "Math.h"

#include <vector>

using namespace Math;

//=============================================================================
// BoxCollision::BoxCollision
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	[Transform *]   Target transform, to perform collision calculations against
//              [XMFLOAT3]      Box dimensions, relative to the center of the transform
// 
// Return:      N/A
// 
//=============================================================================
BoxCollision::BoxCollision(Transform *trans, XMFLOAT3 size) {
    m_Transform = trans;
    m_BoxSize = size;
}

inline void BoxCollision::GetIntersection(float fDst1, float fDst2, XMFLOAT3 P1, XMFLOAT3 P2, XMFLOAT3& Hit) {
    if ((fDst1 * fDst2) >= 0.0f) return;
    if (fDst1 == fDst2) return;
    Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
}

inline bool BoxCollision::InBox(XMFLOAT3 Hit, XMFLOAT3 B1, XMFLOAT3 B2, const int Axis) {
    if (Axis == X && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
    if (Axis == Y && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
    if (Axis == Z && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
    return false;
}

//=============================================================================
// BoxCollision::Ray_Intersect
//=============================================================================
// 
// Description: Performs a line intersection test with this box collider.
// 
// Parameters:	[XMFLOAT3]      Line start point
//              [XMFLOAT3]      Line end point
//              [XMFLOAT3 &]    Output intersection (hit) point.
//                              If function returns false, this parameter remains unchanged.
// 
// Return:      [bool]          Is intersecting?
// 
//=============================================================================
bool BoxCollision::Ray_Intersect(XMFLOAT3 L1, XMFLOAT3 L2, XMFLOAT3& Hit) {
    XMFLOAT3 B1 = m_Transform->GetPosition() - m_BoxSize;
    XMFLOAT3 B2 = m_Transform->GetPosition() + m_BoxSize;

    if (L2.x < B1.x && L1.x < B1.x) return false;
    if (L2.x > B2.x && L1.x > B2.x) return false;
    if (L2.y < B1.y && L1.y < B1.y) return false;
    if (L2.y > B2.y && L1.y > B2.y) return false;
    if (L2.z < B1.z && L1.z < B1.z) return false;
    if (L2.z > B2.z && L1.z > B2.z) return false;
    if (L1.x > B1.x && L1.x < B2.x &&
        L1.y > B1.y && L1.y < B2.y &&
        L1.z > B1.z && L1.z < B2.z)
    {
        Hit = L1;
        return true;
    }

    Box_Hit Hits[MAX_PLANES];

    GetIntersection(L1.x - B1.x, L2.x - B1.x, L1, L2, Hits[NX].point);
    GetIntersection(L1.y - B1.y, L2.y - B1.y, L1, L2, Hits[NY].point);
    GetIntersection(L1.z - B1.z, L2.z - B1.z, L1, L2, Hits[NZ].point);
    GetIntersection(L1.x - B2.x, L2.x - B2.x, L1, L2, Hits[PX].point);
    GetIntersection(L1.y - B2.y, L2.y - B2.y, L1, L2, Hits[PY].point);
    GetIntersection(L1.z - B2.z, L2.z - B2.z, L1, L2, Hits[PZ].point);

    Hits[NX].is_in_box = InBox(Hits[NX].point, B1, B2, X);
    Hits[NY].is_in_box = InBox(Hits[NY].point, B1, B2, Y);
    Hits[NZ].is_in_box = InBox(Hits[NZ].point, B1, B2, Z);
    Hits[PX].is_in_box = InBox(Hits[PX].point, B1, B2, X);
    Hits[PY].is_in_box = InBox(Hits[PY].point, B1, B2, Y);
    Hits[PZ].is_in_box = InBox(Hits[PZ].point, B1, B2, Z);

    std::vector<Box_Hit> Box_Hits;

    for (int i = 0; i < MAX_PLANES; i++) {
        if (Hits[i].is_in_box)
            Box_Hits.push_back(Hits[i]);
    }

    if (Box_Hits.size() > 0) {
        float distance = XMFLOAT3_Distance(L1, Box_Hits[0].point);
        int closest_hit = 0;

        for (unsigned int i = 0; i < Box_Hits.size(); i++) {
            if (XMFLOAT3_Distance(L1, Box_Hits[i].point) < distance)
                closest_hit = i;
        }

        Hit = Box_Hits[closest_hit].point;
        return true;
    }

    return false;
}

//=============================================================================
// BoxCollision::~BoxCollision
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
BoxCollision::~BoxCollision() {

}
