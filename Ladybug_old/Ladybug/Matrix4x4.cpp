
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <memory.h>

#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mathf.h"

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

Matrix4x4::Matrix4x4(const Matrix4x4& rhs)
{
	memcpy(this->m, rhs.m, sizeof(this->m));
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs)
{
	memcpy(this->m, rhs.m, sizeof(this->m));
	return *this;
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

Vector3 Matrix4x4::MultiplyPoint3x4(const Vector3& point)
{
	Vector3 result;
	result.x = m00 * point.x + m01 * point.y + m02 * point.z + m03;
	result.y = m10 * point.x + m11 * point.y + m12 * point.z + m13;
	result.z = m20 * point.x + m21 * point.y + m22 * point.z + m23;
	return result;
}

// https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl
// Reference: glm orthoRH_NO
//   RH: RIGHT_HANDED, For OpenGL
//   NO: NEGATIVE_ONE_TO_ONE
Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4x4 result;
	result.m00 = 2.f / (right - left);
	result.m11 = 2.f / (top - bottom);
	result.m22 = -2.f / (zFar - zNear);
	result.m03 = -(right + left) / (right - left);
	result.m13 = -(top + bottom) / (top - bottom);
	result.m23 = -(zFar + zNear) / (zFar - zNear);
	result.m33 = 1.f;
	return result;
}

// Reference: glm perspectiveRH_NO
// https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl
Matrix4x4 Matrix4x4::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	float tanHalfFovy = tanf((fovy * Mathf::Deg2Rad) / 2);

	Matrix4x4 result;
	result.m00 = 1 / (tanHalfFovy * aspect);
	result.m11 = 1 / (tanHalfFovy);
	result.m22 = -(zFar + zNear) / (zFar - zNear);
	result.m23 = -2 * zFar * zNear / (zFar - zNear);
	result.m32 = -1;

	return result;
}

Matrix4x4 Matrix4x4::inverse() const
{
	// Unreal VectorMatrixInverse
	// https://github.com/EpicGames/UnrealEngine/blob/master/Engine/Source/Runtime/Core/Public/Math/UnrealMathFPU.h

	Matrix4x4 R;
	typedef float Float4x4[4][4];
	const Float4x4& M = this->m;
	Float4x4& Result = R.m;
	float Det[4];
	Float4x4 Tmp;

	Tmp[0][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
	Tmp[0][1] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
	Tmp[0][2] = M[1][2] * M[2][3] - M[1][3] * M[2][2];

	Tmp[1][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
	Tmp[1][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
	Tmp[1][2] = M[0][2] * M[2][3] - M[0][3] * M[2][2];

	Tmp[2][0] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
	Tmp[2][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
	Tmp[2][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	Tmp[3][0] = M[1][2] * M[2][3] - M[1][3] * M[2][2];
	Tmp[3][1] = M[0][2] * M[2][3] - M[0][3] * M[2][2];
	Tmp[3][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	Det[0] = M[1][1] * Tmp[0][0] - M[2][1] * Tmp[0][1] + M[3][1] * Tmp[0][2];
	Det[1] = M[0][1] * Tmp[1][0] - M[2][1] * Tmp[1][1] + M[3][1] * Tmp[1][2];
	Det[2] = M[0][1] * Tmp[2][0] - M[1][1] * Tmp[2][1] + M[3][1] * Tmp[2][2];
	Det[3] = M[0][1] * Tmp[3][0] - M[1][1] * Tmp[3][1] + M[2][1] * Tmp[3][2];

	float Determinant = M[0][0] * Det[0] - M[1][0] * Det[1] + M[2][0] * Det[2] - M[3][0] * Det[3];
	const float	RDet = 1.0f / Determinant;

	Result[0][0] = RDet * Det[0];
	Result[0][1] = -RDet * Det[1];
	Result[0][2] = RDet * Det[2];
	Result[0][3] = -RDet * Det[3];
	Result[1][0] = -RDet * (M[1][0] * Tmp[0][0] - M[2][0] * Tmp[0][1] + M[3][0] * Tmp[0][2]);
	Result[1][1] = RDet * (M[0][0] * Tmp[1][0] - M[2][0] * Tmp[1][1] + M[3][0] * Tmp[1][2]);
	Result[1][2] = -RDet * (M[0][0] * Tmp[2][0] - M[1][0] * Tmp[2][1] + M[3][0] * Tmp[2][2]);
	Result[1][3] = RDet * (M[0][0] * Tmp[3][0] - M[1][0] * Tmp[3][1] + M[2][0] * Tmp[3][2]);
	Result[2][0] = RDet * (
		M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
		M[2][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) +
		M[3][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])
		);
	Result[2][1] = -RDet * (
		M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
		M[2][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
		M[3][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1])
		);
	Result[2][2] = RDet * (
		M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) -
		M[1][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
		M[3][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
		);
	Result[2][3] = -RDet * (
		M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
		);
	Result[3][0] = -RDet * (
		M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
		M[2][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) +
		M[3][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
		);
	Result[3][1] = RDet * (
		M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
		M[2][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
		M[3][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1])
		);
	Result[3][2] = -RDet * (
		M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) -
		M[1][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
		M[3][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
		);
	Result[3][3] = RDet * (
		M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
		);

	return R;
}

Matrix4x4 Matrix4x4::Scale(const Vector3& vector)
{
	Matrix4x4 result;
	result.m00 = vector.x;
	result.m01 = 0.f;
	result.m02 = 0.f;
	result.m03 = 0.f;
	result.m10 = 0.f;
	result.m11 = vector.y;
	result.m12 = 0.f;
	result.m13 = 0.f;
	result.m20 = 0.f;
	result.m21 = 0.f;
	result.m22 = vector.z;
	result.m23 = 0.f;
	result.m30 = 0.f;
	result.m31 = 0.f;
	result.m32 = 0.f;
	result.m33 = 1.f;
	return result;
}

Matrix4x4 Matrix4x4::RotateX(float theta)
{
	Matrix4x4 rot = Matrix4x4::identity;
	rot.m11 = cosf(theta);
	rot.m12 = -sinf(theta);
	rot.m21 = sinf(theta);
	rot.m22 = cosf(theta);
	return rot;
}

Matrix4x4 Matrix4x4::RotateY(float theta)
{
	Matrix4x4 rot = Matrix4x4::identity;
	rot.m00 = cosf(theta);
	rot.m02 = sinf(theta);
	rot.m20 = -sinf(theta);
	rot.m22 = cosf(theta);
	return rot;
}

Matrix4x4 Matrix4x4::RotateZ(float theta)
{
	Matrix4x4 rot = Matrix4x4::identity;
	rot.m00 = cosf(theta);
	rot.m01 = -sinf(theta);
	rot.m10 = sinf(theta);
	rot.m11 = cosf(theta);
	return rot;
}

Matrix4x4 Matrix4x4::Rotate(const Vector3& vector)
{
	Matrix4x4 rot = RotateY(vector.y * Mathf::Deg2Rad) * RotateX(vector.x * Mathf::Deg2Rad) * RotateZ(vector.z * Mathf::Deg2Rad);
	return rot;
}

Matrix4x4 Matrix4x4::Translate(const Vector3& vector)
{
	Matrix4x4 t = Matrix4x4::identity;
	t.m03 = vector.x;
	t.m13 = vector.y;
	t.m23 = vector.z;
	return t;
}
