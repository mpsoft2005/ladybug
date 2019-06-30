
#include <math.h>

#include "Vector3.h"

const Vector3 Vector3::zero(0, 0, 0);
const Vector3 Vector3::one(1, 1, 1);
const Vector3 Vector3::forward(0, 0, 1);
const Vector3 Vector3::back(0, 0, -1);
const Vector3 Vector3::up(0, 1, 0);
const Vector3 Vector3::down(0, -1, 0);
const Vector3 Vector3::left(-1, 0, 0);
const Vector3 Vector3::right(1, 0, 0);

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::~Vector3()
{
}

Vector3& Vector3::operator=(const Vector3 &rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	return *this;
}

Vector3 Vector3::operator/(float d) const
{
	return Vector3(x / d, y / d, z / d);
}

Vector3 Vector3::operator+(const Vector3 &rhs) const
{
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator-(const Vector3 &rhs) const
{
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-this->x, -this->y, -this->z);
}

Vector3 Vector3::operator*(float d) const
{
	return Vector3(x * d, y * d, z * d);
}

Vector3 Vector3::normalized() const
{
	return Normalize(*this);
}

float Vector3::Magnitude(const Vector3& vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

Vector3 Vector3::Normalize(const Vector3& value)
{
	float num = Magnitude(value);
	if (num > 1E-05f)
	{
		return value / num;
	}
	return zero;
}

Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float Vector3::Distance(const Vector3& a, const Vector3& b)
{
	Vector3 vector(a.x - b.x, a.y - b.y, a.z - b.z);
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}
