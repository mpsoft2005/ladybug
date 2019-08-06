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

	Vector3& operator=(const Vector3 &rhs);
	Vector3 operator/(float d) const;
	Vector3 operator+(const Vector3 &rhs) const;
	Vector3 operator-(const Vector3 &rhs) const;
	Vector3 operator-() const;
	Vector3 operator*(float d) const;

	Vector3 normalized() const;

	static float Magnitude(const Vector3& vector);
	static Vector3 Normalize(const Vector3& value);
	static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	static float Dot(const Vector3& lhs, const Vector3& rhs);
	static float Distance(const Vector3& a, const Vector3& b);
};

