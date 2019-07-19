#include "DirectionalLight.h"
#include "Matrix4x4.h"


DirectionalLight::DirectionalLight()
{
	transform = new Transform();
}


DirectionalLight::~DirectionalLight()
{
	delete transform;
}

Vector3 DirectionalLight::getDirection()
{
	Matrix4x4 R = Matrix4x4::Rotate(transform->localEulerAngles);
	Vector3 lightDir = R.MultiplyPoint3x4(-Vector3::forward);
	return lightDir;
}
