#pragma once

#include <memory>

#include "Transform.h"
#include "Matrix4x4.h"
#include "Vector3.h"

class Camera
{
public:
	std::unique_ptr<Transform> transform;

	bool orthographic = false;
	float orthographicSize = 5; // Camera's half-size when in orthographic mode

	float fieldOfView = 60;
	float nearClipPlane = 0.3f;
	float farClipPlane = 1000;

private:
	Vector3 WorldToViewportPoint_Perspective(const Vector3& position);
	Vector3 WorldToScreenPoint_Perspective(const Vector3& position);

	Vector3 WorldToViewportPoint_Ortho(const Vector3& position);
	Vector3 WorldToScreenPoint_Ortho(const Vector3& position);

public:
	Camera();
	virtual ~Camera();

	Matrix4x4 cameraToWorldMatrix();
	Matrix4x4 worldToCameraMatrix();
	Matrix4x4 projectionMatrix();

	Vector3 WorldToViewportPoint(const Vector3& position);
	Vector3 WorldToScreenPoint(const Vector3& position);
};

