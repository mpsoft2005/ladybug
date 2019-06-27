#pragma once

#include "Component.h"
#include "Matrix4x4.h"
#include "Vector3.h"

class Camera : public Component
{
public:
	float fov = 60;
	float near = 0.3f;
	float far = 1000;

public:
	Camera();
	virtual ~Camera();

	Matrix4x4 cameraToWorldMatrix();
	Matrix4x4 worldToCameraMatrix();
	Matrix4x4 projectionMatrix();

	Vector3 WorldToViewportPoint(const Vector3& position);
	Vector3 WorldToScreenPoint(const Vector3& position);
};

