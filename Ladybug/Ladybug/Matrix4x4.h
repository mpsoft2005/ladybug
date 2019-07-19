#pragma once

class Vector3;
class Vector4;

class Matrix4x4
{
public:
	float m[4][4];

	float& m00 = m[0][0];
	float& m10 = m[1][0];
	float& m20 = m[2][0];
	float& m30 = m[3][0];
	float& m01 = m[0][1];
	float& m11 = m[1][1];
	float& m21 = m[2][1];
	float& m31 = m[3][1];
	float& m02 = m[0][2];
	float& m12 = m[1][2];
	float& m22 = m[2][2];
	float& m32 = m[3][2];
	float& m03 = m[0][3];
	float& m13 = m[1][3];
	float& m23 = m[2][3];
	float& m33 = m[3][3];

	static const Matrix4x4 zero;
	static const Matrix4x4 identity;

private:
	static Matrix4x4 RotateX(float theta);
	static Matrix4x4 RotateY(float theta);
	static Matrix4x4 RotateZ(float theta);

public:
	Matrix4x4();
	Matrix4x4(const Vector4& column0, const Vector4& column1, const Vector4& column2, const Vector4& column3);
	~Matrix4x4();

	Matrix4x4(const Matrix4x4& rhs);
	Matrix4x4& operator=(const Matrix4x4& rhs);

	Matrix4x4 operator* (const Matrix4x4& rhs) const;
	Vector4 operator* (const Vector4& vector) const;

	Vector3 MultiplyPoint(const Vector3& point);
	Vector3 MultiplyPoint3x4(const Vector3& point); // it can only handle regular 3D transformations
	static Matrix4x4 Perspective(float fovy, float aspect, float zNear, float zFar);

	Matrix4x4 inverse() const;

	static Matrix4x4 Scale(const Vector3& vector);
	static Matrix4x4 Rotate(const Vector3& vector);
	static Matrix4x4 Translate(const Vector3& vector);
};

