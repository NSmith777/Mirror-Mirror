#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Math {
	float Lerp(float a, float b, float f);

	XMFLOAT3 XMFLOAT3_Lerp(XMFLOAT3 a, XMFLOAT3 b, float f);
	float XMFLOAT3_Distance(XMFLOAT3 a, XMFLOAT3 b);

	XMFLOAT3 operator*(float l, XMFLOAT3 r);
	XMFLOAT3 operator*(XMFLOAT3 l, float r);
	XMFLOAT3 operator/(XMFLOAT3 l, float r);
	XMFLOAT3 operator-(XMFLOAT3 l, XMFLOAT3 r);
	XMFLOAT3 operator+(XMFLOAT3 l, XMFLOAT3 r);
}
