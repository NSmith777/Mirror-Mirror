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
    m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
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
    XMMATRIX scaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    XMMATRIX rotateXMtx = XMMatrixRotationNormal({ 1, 0, 0 }, m_Rotation.x);
    XMMATRIX rotateYMtx = XMMatrixRotationNormal({ 0, 1, 0 }, m_Rotation.y);
    XMMATRIX rotateZMtx = XMMatrixRotationNormal({ 0, 0, 1 }, m_Rotation.z);
    XMMATRIX translateMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

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
