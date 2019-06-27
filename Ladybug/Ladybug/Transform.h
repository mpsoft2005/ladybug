#pragma once

#include "Component.h"
#include "Vector3.h"

class Transform : public Component
{
public:
	Vector3 localPosition;
	Vector3 localEulerAngles;
	Vector3 localScale;

public:
	Transform();
	~Transform();
};

