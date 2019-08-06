#include "Ray.h"

Ray::Ray()
{
	origin = Vector3::zero;
	direction = Vector3::forward;
}

Ray::Ray(const Vector3& origin, const Vector3& direction)
{
	this->origin = origin;
	this->direction = direction.normalized();
}


Ray::~Ray()
{
}
