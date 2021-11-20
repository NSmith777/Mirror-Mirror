#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Transform {
public:
	Transform();
	~Transform();

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }

	XMMATRIX GetModelMatrix();

	XMFLOAT3 GetRightVec();
	XMFLOAT3 GetUpVec();
	XMFLOAT3 GetForwardVec();

private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
};
