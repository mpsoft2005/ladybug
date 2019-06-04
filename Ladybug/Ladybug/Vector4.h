#pragma once
class Vector4
{
public:
	const float kEpsilon = 1E-05f;
	float x;
	float y;
	float z;
	float w;

public:
	Vector4();
	Vector4(float x, float y, float z, float w);
	~Vector4();
};

