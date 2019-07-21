#include "Light.h"
#include "Matrix4x4.h"


Light::Light()
{
	transform = std::make_unique<Transform>();
}


Light::~Light()
{

}

Vector3 Light::getDirection()
{
	Matrix4x4 R = Matrix4x4::Rotate(transform->localEulerAngles);
	Vector3 lightDir = R.MultiplyPoint3x4(-Vector3::forward);
	return lightDir;
}
