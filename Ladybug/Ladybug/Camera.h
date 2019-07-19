#pragma once

#include "Transform.h"
#include "Matrix4x4.h"
#include "Vector3.h"

class Camera
{
public:
	Transform *transform = 0;

	bool orthographic = false;
	float orthographicSize = 5;

	float fieldOfView = 60;
	float nearClipPlane = 0.3f;
	float farClipPlane = 1000;

public:
	Camera();
	virtual ~Camera();

	Matrix4x4 cameraToWorldMatrix();
	Matrix4x4 worldToCameraMatrix();
	Matrix4x4 projectionMatrix();

	Vector3 WorldToViewportPoint(const Vector3& position);
	Vector3 WorldToScreenPoint(const Vector3& position);
};

