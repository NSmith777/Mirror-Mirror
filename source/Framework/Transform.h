//==============================================================================
// File: Transform.h
// 
// Description: Declares the Transform component.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "Math.h"

using namespace DirectX;
using namespace Math;

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Transform {
public:
	Transform();
	~Transform();

	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }

	void SetPosition(XMFLOAT3 new_position) { position = new_position; }
	void SetRotation(XMFLOAT3 new_rotation) { rotation = new_rotation; }
	void SetScale(XMFLOAT3 new_scale) { scale = new_scale; }

	void Translate(XMFLOAT3 translate) { position = position + translate; }
	void Rotate(XMFLOAT3 rotate) { rotation = rotation + rotate; }

	XMMATRIX GetModelMatrix();

	XMFLOAT3 GetRightVec();
	XMFLOAT3 GetUpVec();
	XMFLOAT3 GetForwardVec();

private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
};
