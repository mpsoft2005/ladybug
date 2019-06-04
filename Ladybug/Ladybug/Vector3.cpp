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
