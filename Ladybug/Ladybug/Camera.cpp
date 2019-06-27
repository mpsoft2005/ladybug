#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

Matrix4x4 Camera::cameraToWorldMatrix()
{
	Transform* t = gameObject->transform;
	Matrix4x4 translate = Matrix4x4::Translate(t->localPosition);
	Matrix4x4 rotate = Matrix4x4::Rotate(t->localEulerAngles);
	Matrix4x4 scale = Matrix4x4::Scale(Vector3(t->localScale.x, t->localScale.y, -t->localScale.z));
	return translate * rotate * scale;
}

Matrix4x4 Camera::worldToCameraMatrix()
{
	Matrix4x4 cameraToWorld = cameraToWorldMatrix();
	return cameraToWorld.inverse();
}
