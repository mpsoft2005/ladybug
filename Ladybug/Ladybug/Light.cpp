#include "Light.h"
#include "Matrix4x4.h"


Light::Light()
{
	transform = new Transform();
}


Light::~Light()
{
	delete transform;
}

Vector3 Light::getDirection()
{
	Matrix4x4 R = Matrix4x4::Rotate(transform->localEulerAngles);
	Vector3 lightDir = R.MultiplyPoint3x4(-Vector3::forward);
	return lightDir;
}
