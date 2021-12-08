//==============================================================================
// File: Transform.cpp
// 
// Description: Implements the Transform component.
// 
//==============================================================================

#include "Transform.h"

//=============================================================================
// Transform::Transform
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Transform::Transform() {
    position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
    scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// Transform::GetModelMatrix
//=============================================================================
// 
// Description: Constructs a model matrix using this transform.
// 
// Parameters:	N/A
// 
// Return:      [XMMATRIX]	This transform's model matrix
// 
//=============================================================================
XMMATRIX Transform::GetModelMatrix() {
    XMMATRIX scaleMtx = XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX rotateXMtx = XMMatrixRotationNormal({ 1, 0, 0 }, rotation.x);
    XMMATRIX rotateYMtx = XMMatrixRotationNormal({ 0, 1, 0 }, rotation.y);
    XMMATRIX rotateZMtx = XMMatrixRotationNormal({ 0, 0, 1 }, rotation.z);
    XMMATRIX translateMtx = XMMatrixTranslation(position.x, position.y, position.z);

    return scaleMtx * rotateXMtx * rotateYMtx * rotateZMtx * translateMtx;
}

//=============================================================================
// Transform::GetRightVec
//=============================================================================
// 
// Description: Extracts the right direction vector from this transform.
// 
// Parameters:	N/A
// 
// Return:      [XMFLOAT3]	This transform's normalized right direction
// 
//=============================================================================
XMFLOAT3 Transform::GetRightVec() {
    XMFLOAT3 ret;
    XMStoreFloat3(&ret, GetModelMatrix().r[0]);
    return ret;
}

//=============================================================================
// Transform::GetUpVec
//=============================================================================
// 
// Description: Extracts the up direction vector from this transform.
// 
// Parameters:	N/A
// 
// Return:      [XMFLOAT3]	This transform's normalized up direction
// 
//=============================================================================
XMFLOAT3 Transform::GetUpVec() {
    XMFLOAT3 ret;
    XMStoreFloat3(&ret, GetModelMatrix().r[1]);
    return ret;
}

//=============================================================================
// Transform::GetForwardVec
//=============================================================================
// 
// Description: Extracts the forward direction vector from this transform.
// 
// Parameters:	N/A
// 
// Return:      [XMFLOAT3]	This transform's normalized forward direction
// 
//=============================================================================
XMFLOAT3 Transform::GetForwardVec() {
    XMFLOAT3 ret;
    XMStoreFloat3(&ret, GetModelMatrix().r[2]);
    return ret;
}

//=============================================================================
// Transform::~Transform
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Transform::~Transform() {

}
