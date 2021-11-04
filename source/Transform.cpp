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

Transform::~Transform() {

}
