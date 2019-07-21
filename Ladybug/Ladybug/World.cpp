#include "World.h"

#include "Color.h"
#include "Screen.h"
#include "Camera.h"
#include "Light.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Mathf.h"


World::World()
{

}


World::~World()
{

}

// calc signed area of parallelogram
static inline float edgeFunction(const Vector3& a, const Vector3& b, const Vector3& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

static Vector3 InterpolateVertexAttribute(
	const Vector3& attr0, const Vector3& attr1, const Vector3& attr2,
	float z0, float z1, float z2,
	float w0, float w1, float w2)
{
	Vector3 attr;
	attr = attr0 * w0 / z0 + attr1 * w1 / z1 + attr2 * w2 / z2;
	attr = attr / (w0 / z0 + w1 / z1 + w2 / z2);
	return attr;
}

// Reference: glm reflect
// https://github.com/g-truc/glm/blob/master/glm/detail/func_geometric.inl
static Vector3 reflect(const Vector3& I, const Vector3& N)
{
	return I - N * Vector3::Dot(N, I) * 2.f;
}

static inline float saturate(float v)
{
	if (v > 1)
		return 1;
	if (v < 0)
		return 0;
	return v;
}

void World::PrepareBuffers()
{
	if (frameBuffer == nullptr)
	{
		frameBuffer = std::make_unique<Color[]>(Screen::width * Screen::height);
	}

	if (depthBuffer == nullptr)
	{
		depthBuffer = std::make_unique<float[]>(Screen::width * Screen::height);
	}
}

void World::Render()
{
	PrepareBuffers();

	for (int i = 0; i < Screen::width * Screen::height; i++)
	{
		// clear frame buffer
		frameBuffer[i].r = 49 / 255.0f;
		frameBuffer[i].g = 77 / 255.0f;
		frameBuffer[i].b = 121 / 255.0f;
		// clear z buffer
		depthBuffer[i] = camera->farClipPlane;
	}

	Pipeline pipe;
	pipe.RegisterListener(this);
	pipe.Process(*this, depthBuffer.get(), frameBuffer.get(), Screen::width, Screen::height);
}

v2f World::OnProcessVertex(const Pipeline& pipe, const a2v& in)
{
	v2f out;

	out.pos = camera->WorldToScreenPoint(in.vertex);
	out.normal = in.normal;
	out.worldPos = in.vertex;

	return out;
}

Color World::OnProcessFragment(const Pipeline& pipe, const v2f& in)
{
	Material* material = in.material.get();
	Color ambient(0, 0, 0); // ambient color
	Color lightColor = light->color * light->intensity;

	// light direction
	Vector3 L = light->getDirection();
	Vector3 N = in.normal;

	Vector3 vWorld = in.worldPos;

	Vector3 reflectDir = reflect(-L, N).normalized();
	Vector3 viewDir = (camera->transform->localPosition - vWorld).normalized();

	Color diffuse = material->albedo * lightColor * std::max(0.f, Vector3::Dot(N, L));
	Color specular = material->specular * lightColor * pow(saturate(Vector3::Dot(reflectDir, viewDir)), material->specularGloss);

	return ambient + diffuse + specular;
}
