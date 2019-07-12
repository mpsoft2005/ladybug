#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
#include "Screen.h"
#include "Vector4.h"

Camera::Camera()
{
	transform = new Transform();
}


Camera::~Camera()
{
	delete transform;
}

Matrix4x4 Camera::cameraToWorldMatrix()
{
	Transform* t = transform;
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

Matrix4x4 Camera::projectionMatrix()
{
	float aspect = (float)Screen::width / Screen::height;
	return Matrix4x4::Perspective(fov, aspect, near, far);
}

Vector3 Camera::WorldToViewportPoint(const Vector3& pos)
{
	// TODO: What if Camera property changed?
	static Matrix4x4 view = worldToCameraMatrix();
	static Matrix4x4 projection = projectionMatrix();
	static Matrix4x4 mvp = projection * view;

	Vector4 v1(pos.x, pos.y, pos.z, 1);
	Vector4 v2 = mvp * v1;

	Vector3 vNDC;
	vNDC.x = v2.x / v2.w;
	vNDC.y = v2.y / v2.w;

	float viewportX = (vNDC.x + 1) / 2;
	float viewportY = (vNDC.y + 1) / 2;

	return Vector3(viewportX, viewportY, v2.w);
}

Vector3 Camera::WorldToScreenPoint(const Vector3& pos)
{
	Vector3 viewportPoint = WorldToViewportPoint(pos);
	return Vector3(viewportPoint.x * Screen::width, viewportPoint.y * Screen::height, viewportPoint.z);
}
