#include "Math.h"

namespace Math {
    float Lerp(float a, float b, float f) {
        return a + f * (b - a);
    }

    XMFLOAT3 XMFLOAT3_Lerp(XMFLOAT3 a, XMFLOAT3 b, float f) {
        XMFLOAT3 ret;
        ret.x = Lerp(a.x, b.x, f);
        ret.y = Lerp(a.y, b.y, f);
        ret.z = Lerp(a.z, b.z, f);
        return ret;
    }

    float XMFLOAT3_Distance(XMFLOAT3 a, XMFLOAT3 b) {
        return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2) + powf(b.z - a.z, 2));
    }

    XMFLOAT3 operator*(float l, XMFLOAT3 r) {
        XMFLOAT3 ret;
        ret.x = l * r.x;
        ret.y = l * r.y;
        ret.z = l * r.z;
        return ret;
    }

    XMFLOAT3 operator*(XMFLOAT3 l, float r) {
        XMFLOAT3 ret;
        ret.x = l.x * r;
        ret.y = l.y * r;
        ret.z = l.z * r;
        return ret;
    }

    XMFLOAT3 operator/(XMFLOAT3 l, float r) {
        XMFLOAT3 ret;
        ret.x = l.x / r;
        ret.y = l.y / r;
        ret.z = l.z / r;
        return ret;
    }

    XMFLOAT3 operator-(XMFLOAT3 l, XMFLOAT3 r) {
        XMFLOAT3 ret;
        ret.x = l.x - r.x;
        ret.y = l.y - r.y;
        ret.z = l.z - r.z;
        return ret;
    }

    XMFLOAT3 operator+(XMFLOAT3 l, XMFLOAT3 r) {
        XMFLOAT3 ret;
        ret.x = l.x + r.x;
        ret.y = l.y + r.y;
        ret.z = l.z + r.z;
        return ret;
    }
}
