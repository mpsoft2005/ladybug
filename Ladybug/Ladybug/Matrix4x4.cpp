
#include <math.h>
#include <assert.h>

#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"

const Matrix4x4 Matrix4x4::zero(Vector4(0, 0, 0, 0), Vector4(0, 0, 0, 0), Vector4(0, 0, 0, 0), Vector4(0, 0, 0, 0));
const Matrix4x4 Matrix4x4::identity(Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), Vector4(0, 0, 0, 1));

Matrix4x4::Matrix4x4()
{
	m00 = 0;
	m01 = 0;
	m02 = 0;
	m03 = 0;
	m10 = 0;
	m11 = 0;
	m12 = 0;
	m13 = 0;
	m20 = 0;
	m21 = 0;
	m22 = 0;
	m23 = 0;
	m30 = 0;
	m31 = 0;
	m32 = 0;
	m33 = 0;
}

Matrix4x4::Matrix4x4(const Vector4& column0, const Vector4& column1, const Vector4& column2, const Vector4& column3)
{
	m00 = column0.x;
	m01 = column1.x;
	m02 = column2.x;
	m03 = column3.x;
	m10 = column0.y;
	m11 = column1.y;
	m12 = column2.y;
	m13 = column3.y;
	m20 = column0.z;
	m21 = column1.z;
	m22 = column2.z;
	m23 = column3.z;
	m30 = column0.w;
	m31 = column1.w;
	m32 = column2.w;
	m33 = column3.w;
}

Matrix4x4::~Matrix4x4()
{
}

Matrix4x4 Matrix4x4::operator* (const Matrix4x4& rhs) const
{
	Matrix4x4 result;
	const Matrix4x4& lhs = *this;
	result.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30;
	result.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31;
	result.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32;
	result.m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33;
	result.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30;
	result.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
	result.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
	result.m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33;
	result.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30;
	result.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
	result.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
	result.m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;
	result.m30 = lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30;
	result.m31 = lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31;
	result.m32 = lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32;
	result.m33 = lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;
	return result;
}

Vector4 Matrix4x4::operator* (const Vector4& vector) const
{
	Vector4 result;
	const Matrix4x4& lhs = *this;
	result.x = lhs.m00 * vector.x + lhs.m01 * vector.y + lhs.m02 * vector.z + lhs.m03 * vector.w;
	result.y = lhs.m10 * vector.x + lhs.m11 * vector.y + lhs.m12 * vector.z + lhs.m13 * vector.w;
	result.z = lhs.m20 * vector.x + lhs.m21 * vector.y + lhs.m22 * vector.z + lhs.m23 * vector.w;
	result.w = lhs.m30 * vector.x + lhs.m31 * vector.y + lhs.m32 * vector.z + lhs.m33 * vector.w;
	return result;
}

Vector3 Matrix4x4::MultiplyPoint(const Vector3& point)
{
	Vector3 result;
	result.x = m00 * point.x + m01 * point.y + m02 * point.z + m03;
	result.y = m10 * point.x + m11 * point.y + m12 * point.z + m13;
	result.z = m20 * point.x + m21 * point.y + m22 * point.z + m23;
	float num = m30 * point.x + m31 * point.y + m32 * point.z + m33;
	num = 1.0f / num;
	result.x *= num;
	result.y *= num;
	result.z *= num;
	return result;
}

static inline float Radians(float degree)
{
	return degree * (float)M_PI / 180;
}

Matrix4x4 Matrix4x4::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	float tanHalfFovy = tanf(Radians(fovy) / 2);

	Matrix4x4 result;
	result.m00 = 1 / (tanHalfFovy * aspect);
	result.m11 = 1 / (tanHalfFovy);
	result.m22 = -(zFar + zNear) / (zFar - zNear);
	result.m23 = -2 * zFar*zNear / (zFar - zNear);
	result.m32 = -1;

	return result;
}
