#pragma once

#include "Component.h"
#include "Matrix4x4.h"

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	Matrix4x4 cameraToWorldMatrix();
	Matrix4x4 worldToCameraMatrix();
};

