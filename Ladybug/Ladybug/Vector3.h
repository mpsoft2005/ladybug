#pragma once
class Vector3
{
public:
	const float kEpsilon = 1E-05f;
	float x;
	float y;
	float z;

	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 forward;
	static const Vector3 back;
	static const Vector3 up;
	static const Vector3 down;
	static const Vector3 left;
	static const Vector3 right;

public:
	Vector3();
	Vector3(float x, float y, float z);
	~Vector3();
};

