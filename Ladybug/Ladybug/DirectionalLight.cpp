#include "DirectionalLight.h"



DirectionalLight::DirectionalLight()
{
	transform = new Transform();
}


DirectionalLight::~DirectionalLight()
{
	delete transform;
}
