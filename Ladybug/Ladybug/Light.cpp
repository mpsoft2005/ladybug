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

Vector3 Light::GetDirection()
{
	Matrix4x4 R = Matrix4x4::Rotate(transform->localEulerAngles);
	Vector3 lightDir = R.MultiplyPoint3x4(-Vector3::forward);
	return lightDir;
}

void Light::PrepareShadowMap(const World& world)
{
	if (shadowMap == nullptr)
	{
		shadowMap = std::make_shared<ShadowMap>(*this, world.gameObjects);
	}
}

std::shared_ptr<ShadowMap> Light::GetShadowMap(const World& world)
{
	PrepareShadowMap(world);
	return shadowMap;
}

float Light::ShadowFactor(const World& world, const Vector3& v)
{
	if (shadows == LightShadows::SHADOW_NONE)
		return 1.f;

	PrepareShadowMap(world);
	return shadowMap->ShadowFactor(v);
}
