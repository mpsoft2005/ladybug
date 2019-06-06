#pragma once

class Vector3;
class Vector4;

class Matrix4x4
{
public:
	float m00;
	float m10;
	float m20;
	float m30;
	float m01;
	float m11;
	float m21;
	float m31;
	float m02;
	float m12;
	float m22;
	float m32;
	float m03;
	float m13;
	float m23;
	float m33;

	static const Matrix4x4 zero;
	static const Matrix4x4 identity;

public:
	Matrix4x4();
	Matrix4x4(const Vector4& column0, const Vector4& column1, const Vector4& column2, const Vector4& column3);
	~Matrix4x4();

	Matrix4x4 operator* (const Matrix4x4& rhs);
	Vector4 operator* (const Vector4& vector);

	Vector3 MultiplyPoint(const Vector3& point);
	static Matrix4x4 Perspective(float fovy, float aspect, float zNear, float zFar);
};

