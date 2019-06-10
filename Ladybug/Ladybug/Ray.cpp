#include "Ray.h"



Ray::Ray(const Vector3& origin, const Vector3& direction)
{
	this->origin = origin;
	this->direction = direction.normalized();
}


Ray::~Ray()
{
}
