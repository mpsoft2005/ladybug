#include "Light.h"
#include "Matrix4x4.h"
#include "World.h"


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

float Light::ShadowFactor(const World& world, const Vector3& v)
{
	if (shadows == LightShadows::SHADOW_NONE)
		return 1.f;

	if (shadowMap == nullptr)
	{
		shadowMap = std::make_unique<ShadowMap>(*this);
		shadowMap->Render(world.gameObjects);
	}
	return shadowMap->ShadowFactor(v);
}
