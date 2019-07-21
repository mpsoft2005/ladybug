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
	delete camera;
	delete light;

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
	delete[] frameBuffer;
	delete[] depthBuffer;
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

void World::Render()
{
	if (frameBuffer == NULL)
	{
		frameBuffer = new Color[Screen::width * Screen::height];
	}

	if (depthBuffer == NULL)
	{
		depthBuffer = new float[Screen::width * Screen::height];
	}

	for (int i = 0; i < Screen::width * Screen::height; i++)
	{
		// clear frame buffer
		frameBuffer[i].r = 49 / 255.0f;
		frameBuffer[i].g = 77 / 255.0f;
		frameBuffer[i].b = 121 / 255.0f;
		// clear z buffer
		depthBuffer[i] = camera->farClipPlane;
	}

	// light direction
	Vector3 L = light->getDirection();

	Color ambient(0, 0, 0); // ambient color
	Color lightColor = light->color * light->intensity;

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		Mesh* mesh = gameObjects[i]->mesh;
		Material* material = gameObjects[i]->material;
		size_t numTris = mesh->triangles.size() / 3;

		// OpenGL Rasterization Algorithm
		// https://en.wikibooks.org/wiki/GLSL_Programming/Rasterization

		// Mesa 3D Rasterizer
		// https://github.com/anholt/mesa/blob/master/src/gallium/docs/source/cso/rasterizer.rst

		// Triangle rasterization in practice
		// https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/

		for (size_t idx = 0; idx < numTris; ++idx)
		{
			int i0 = mesh->triangles[idx * 3];
			int i1 = mesh->triangles[idx * 3 + 1];
			int i2 = mesh->triangles[idx * 3 + 2];

			const Vector3& v0World = mesh->vertices[i0];
			const Vector3& v1World = mesh->vertices[i1];
			const Vector3& v2World = mesh->vertices[i2];

			Vector3 v0Raster = camera->WorldToScreenPoint(v0World);
			Vector3 v1Raster = camera->WorldToScreenPoint(v1World);
			Vector3 v2Raster = camera->WorldToScreenPoint(v2World);

			float z0 = v0Raster.z;
			float z1 = v1Raster.z;
			float z2 = v2Raster.z;

			Vector3 e0 = v2Raster - v1Raster;
			Vector3 e1 = v0Raster - v2Raster;
			Vector3 e2 = v1Raster - v0Raster;

			float area = edgeFunction(v0Raster, v1Raster, v2Raster);

			float xmin = Mathf::Min(v0Raster.x, v1Raster.x, v2Raster.x);
			float ymin = Mathf::Min(v0Raster.y, v1Raster.y, v2Raster.y);
			float xmax = Mathf::Max(v0Raster.x, v1Raster.x, v2Raster.x);
			float ymax = Mathf::Max(v0Raster.y, v1Raster.y, v2Raster.y);

			int x0 = std::max(0, (int)(std::roundf(xmin)));
			int x1 = std::min(Screen::width - 1, (int)(std::roundf(xmax)));
			int y0 = std::max(0, (int)(std::roundf(ymin)));
			int y1 = std::min(Screen::height - 1, (int)(std::roundf(ymax)));

			for (int y = y0; y <= y1; y++)
			{
				for (int x = x0; x <= x1; x++)
				{
					Vector3 pixelSample(x + 0.5f, y + 0.5f, 0);
					float w0 = edgeFunction(v1Raster, v2Raster, pixelSample);
					float w1 = edgeFunction(v2Raster, v0Raster, pixelSample);
					float w2 = edgeFunction(v0Raster, v1Raster, pixelSample);

					// Rasterization Rules: top-left rule
					// inside the triangle or
					//   1. lies on triangle top edge
					//   2. lies on triangle left edge
					bool overlaps = true;
					overlaps &= (w0 == 0 ? ((e0.y == 0 && e0.x < 0) || e0.y < 0) : (w0 > 0));
					overlaps &= (w1 == 0 ? ((e1.y == 0 && e1.x < 0) || e1.y < 0) : (w1 > 0));
					overlaps &= (w2 == 0 ? ((e2.y == 0 && e2.x < 0) || e2.y < 0) : (w2 > 0));

					if (overlaps)
					{
						w0 /= area; w1 /= area; w2 /= area;
						float z = 1.f / (w0 / v0Raster.z + w1 / v1Raster.z + w2 / v2Raster.z);

						int idx = y * Screen::width + x;
						if (depthBuffer[idx] > z)
						{
							depthBuffer[idx] = z;

							// Transforming Normal Vectors
							// https://en.wikibooks.org/wiki/GLSL_Programming/Applying_Matrix_Transformations

							const Vector3& N0 = mesh->normals[i0];
							const Vector3& N1 = mesh->normals[i1];
							const Vector3& N2 = mesh->normals[i2];

							Vector3 N = InterpolateVertexAttribute(N0, N1, N2, z0, z1, z2, w0, w1, w2);

							// Remember that an interpolated normal is typically not normalized?
							N = N.normalized();

							Vector3 vWorld = InterpolateVertexAttribute(v0World, v1World, v2World, z0, z1, z2, w0, w1, w2);

							Vector3 reflectDir = reflect(-L, N).normalized();
							Vector3 viewDir = (camera->transform->localPosition - vWorld).normalized();

							Color diffuse = material->albedo * lightColor * std::max(0.f, Vector3::Dot(N, L));
							Color specular = material->specular * lightColor * pow(saturate(Vector3::Dot(reflectDir, viewDir)), material->specularGloss);

							frameBuffer[idx] = ambient + diffuse + specular;
						}
					}
				}
			}
		}
	}
}