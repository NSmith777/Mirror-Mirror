#include "Transform.h"

Transform::Transform() {
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void Transform::Translate(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
}

void Transform::Rotate(float x, float y, float z) {
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

void Transform::SetPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::SetRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void Transform::SetScale(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

XMMATRIX Transform::GetModelMatrix() {
	XMMATRIX scaleMtx = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX rotateXMtx = XMMatrixRotationNormal({ 1, 0, 0 }, rotation.x);
	XMMATRIX rotateYMtx = XMMatrixRotationNormal({ 0, 1, 0 }, rotation.y);
	XMMATRIX rotateZMtx = XMMatrixRotationNormal({ 0, 0, 1 }, rotation.z);
	XMMATRIX translateMtx = XMMatrixTranslation(position.x, position.y, position.z);

	return scaleMtx * rotateXMtx * rotateYMtx * rotateZMtx * translateMtx;
}

XMFLOAT3 Transform::GetRightVec() {
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, GetModelMatrix().r[0]);
	return ret;
}

XMFLOAT3 Transform::GetUpVec() {
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, GetModelMatrix().r[1]);
	return ret;
}

XMFLOAT3 Transform::GetForwardVec() {
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, GetModelMatrix().r[2]);
	return ret;
}

Transform::~Transform() {

}
