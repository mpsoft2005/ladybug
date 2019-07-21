#include <algorithm>
#include <math.h>

#include "Pipeline.h"

#include "World.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Mathf.h"
#include "World.h"
#include "Camera.h"

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

Pipeline::Pipeline()
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::RegisterListener(PipelineListener* listener)
{
	this->listener = listener;
}

// OpenGL Rasterization Algorithm
//   https://en.wikibooks.org/wiki/GLSL_Programming/Rasterization
//
// Mesa 3D Rasterizer
//   https://github.com/anholt/mesa/blob/master/src/gallium/docs/source/cso/rasterizer.rst
//
// Triangle rasterization in practice
//   https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
//
void Pipeline::Process(const World& world, float *depthBuffer, Color *frameBuffer, int screenWidth, int screenHeight)
{
	bool orthographic = world.camera->orthographic;

	for (size_t i = 0; i < world.gameObjects.size(); i++)
	{
		std::shared_ptr<Mesh> mesh = world.gameObjects[i]->mesh;
		std::shared_ptr<Material> material = world.gameObjects[i]->material;
		size_t numTris = mesh->triangles.size() / 3;

		for (size_t idx = 0; idx < numTris; ++idx)
		{
			v2f f[3];
			Vector3 v[3];

			for (int k = 0; k < 3; k++)
			{
				int vertIdx = mesh->triangles[idx * 3 + k];

				a2v in;
				in.vertex = mesh->vertices[vertIdx];
				in.normal = mesh->normals[vertIdx];

				v[k] = in.vertex;
				f[k] = listener->OnProcessVertex(*this, in);
			}

			Vector3 v0Raster = f[0].pos;
			Vector3 v1Raster = f[1].pos;
			Vector3 v2Raster = f[2].pos;

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
			int x1 = std::min(screenWidth - 1, (int)(std::roundf(xmax)));
			int y0 = std::max(0, (int)(std::roundf(ymin)));
			int y1 = std::min(screenHeight - 1, (int)(std::roundf(ymax)));

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

						float z = 0;
						if (orthographic)
						{
							z = w0 * z0 + w1 * z1 + w2 * z2;
						}
						else
						{
							z = 1.f / (w0 / v0Raster.z + w1 / v1Raster.z + w2 / v2Raster.z);
						}

						int idx = y * screenWidth + x;
						if (depthBuffer[idx] > z)
						{
							depthBuffer[idx] = z;
							Vector3 N = InterpolateVertexAttribute(f[0].normal, f[1].normal, f[2].normal, z0, z1, z2, w0, w1, w2);
							Vector3 vWorld = InterpolateVertexAttribute(v[0], v[1], v[2], z0, z1, z2, w0, w1, w2);

							if (frameBuffer != nullptr)
							{
								v2f f;
								f.pos = v0Raster * w0 + v1Raster * w1 + v2Raster * w2;
								f.normal = N.normalized();
								f.worldPos = vWorld;
								f.material = material;
								frameBuffer[idx] = listener->OnProcessFragment(*this, f);
							}
						}
					}
				}
			}
		}
	}
}
