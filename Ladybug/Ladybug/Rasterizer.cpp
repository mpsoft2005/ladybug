
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <vector>

#include "Rasterizer.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Bitmap.h"
#include "Color.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "Test.h"
#include "SVG.h"
#include "ObjLoader.h"
#include "Light.h"
#include "Screen.h"
#include "Camera.h"
#include "Mathf.h"
#include "World.h"
#include "ShadowMap.h"

static Matrix4x4 modelMatrix = Matrix4x4::identity;

static Matrix4x4 viewMatrix(
	Vector4(0.65631f, -0.35794f, 0.66418f, 0.00000f),
	Vector4(0.00000f, 0.88030f, 0.47441f, 0.00000f),
	Vector4(0.75449f, 0.31136f, -0.57775f, 0.00000f),
	Vector4(0.00000f, 0.00000f, -11.26372f, 1.00000f)
);

static Matrix4x4 projectionMatrix(
	Vector4(1.81066f, 0, 0, 0),
	Vector4(0, 2.41421f, 0, 0),
	Vector4(0, 0, -1.00060f, -1),
	Vector4(0, 0, -0.60018f, 0)
);

static Matrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

const int& screenWidth = Screen::width;
const int& screenHeight = Screen::height;

static float nearClipping = 0.3f;
static float farClipping = 1000.0f;

Vector3 WorldToViewportPoint(Vector3 pos)
{
	Vector4 v1(pos.x, pos.y, pos.z, 1);
	Vector4 v2 = mvpMatrix * v1;

	Vector3 vNDC;
	vNDC.x = v2.x / v2.w;
	vNDC.y = v2.y / v2.w;

	float viewportX = (vNDC.x + 1) / 2;
	float viewportY = (vNDC.y + 1) / 2;

	return Vector3(viewportX, viewportY, v2.w);
}

Vector3 WorldToScreenPoint(Vector3 pos)
{
	Vector3 viewportPoint = WorldToViewportPoint(pos);
	return Vector3(viewportPoint.x * screenWidth, viewportPoint.y * screenHeight, viewportPoint.z);
}

// calc signed area of parallelogram
inline float edgeFunction(const Vector3& a, const Vector3& b, const Vector3& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void OutputBitmap(Color *frameBuffer, const char* filename)
{
	Bitmap bitmap(screenWidth, screenHeight);
	for (int y = 0; y < screenHeight; y++)
	{
		for (int x = 0; x < screenWidth; x++)
		{
			const Color& pixel = frameBuffer[y * screenWidth + x];
			bitmap.SetPixel(x, y, pixel);
		}
	}
	bitmap.Save(filename);
}

void OutputDepthBuffer(float *depthBuffer, float near, float far, const char* filename)
{
	Bitmap bitmap(screenWidth, screenHeight);
	for (int y = 0; y < screenHeight; y++)
	{
		for (int x = 0; x < screenWidth; x++)
		{
			float z = depthBuffer[y * screenWidth + x];
			float normalized = (z - near) / (far - near);
			if (normalized >= 0 && normalized <= 1)
			{
				Color c(normalized, normalized, normalized);
				bitmap.SetPixel(x, y, c);
			}
		}
	}
	bitmap.Save(filename);
}

Vector3 InterpolateVertexAttribute(
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
Vector3 reflect(const Vector3& I, const Vector3& N)
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

void test_Rasterization()
{
	Color *frameBuffer = new Color[screenWidth * screenHeight];
	float *depthBuffer = new float[screenWidth * screenHeight];

	for (int i = 0; i < screenWidth * screenHeight; i++)
	{
		// clear frame buffer
		frameBuffer[i].r = 49 / 255.0f;
		frameBuffer[i].g = 77 / 255.0f;
		frameBuffer[i].b = 121 / 255.0f;
		// clear z buffer
		depthBuffer[i] = farClipping;
	}

	// setup lights
	Light light;
	light.color = Color(1, 244 / 255.f, 214 / 255.f);
	light.intensity = 1;
	Vector3 L(0.3213938f, 0.7660444f, -0.5566705f); // light direction

	Color ambient(0, 0, 0); // ambient color

	// setup game objects
	GameObject* object;
	std::vector<GameObject*> gameObjects;

	object = new GameObject();
	object->mesh = ObjLoader::Load("plane_1.obj");
	object->material = new Material();
	object->material->albedo = Color(19 / 255.f, 1, 0);
	gameObjects.push_back(object);

	object = new GameObject();
	object->mesh = ObjLoader::Load("ico-sphere_1-flat.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	gameObjects.push_back(object);

	object->mesh->Debug();

	object = new GameObject();
	object->mesh = ObjLoader::Load("special-cube.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	gameObjects.push_back(object);

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

			Vector3 v0Raster = WorldToScreenPoint(v0World);
			Vector3 v1Raster = WorldToScreenPoint(v1World);
			Vector3 v2Raster = WorldToScreenPoint(v2World);

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

					if (w0 >= 0 && w1 >= 0 && w2 >= 0)
					{
						w0 /= area; w1 /= area; w2 /= area;
						float z = v0Raster.z * w0 + v1Raster.z * w1 + v2Raster.z * w2;

						int idx = y * screenWidth + x;
						if (depthBuffer[idx] > z)
						{
							depthBuffer[idx] = z;
							Vector3 N = mesh->normals[i0];
							Color diffuse = material->albedo * (light.color * light.intensity) * std::max(0.f, Vector3::Dot(N, L));
							frameBuffer[idx] = diffuse + ambient;
						}
					}
				}
			}
		}
	}

	OutputBitmap(frameBuffer, 0);

	SVG svg(screenWidth, screenHeight);
	svg.DrawRect(0, 0, (float)screenWidth, (float)screenHeight, Color32(18, 18, 18), 1, Color32(200, 200, 200));

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		Mesh* mesh = gameObjects[i]->mesh;
		size_t numTris = mesh->triangles.size() / 3;

		for (size_t idx = 0; idx < numTris; ++idx) {
			const Vector3& v0World = mesh->vertices[mesh->triangles[idx * 3]];
			const Vector3& v1World = mesh->vertices[mesh->triangles[idx * 3 + 1]];
			const Vector3& v2World = mesh->vertices[mesh->triangles[idx * 3 + 2]];

			Vector3 v0Raster = WorldToScreenPoint(v0World);
			Vector3 v1Raster = WorldToScreenPoint(v1World);
			Vector3 v2Raster = WorldToScreenPoint(v2World);

			svg.DrawLine(v0Raster.x, screenHeight - v0Raster.y, v1Raster.x, screenHeight - v1Raster.y, Color32(0, 0, 0), 1);
			svg.DrawLine(v1Raster.x, screenHeight - v1Raster.y, v2Raster.x, screenHeight - v2Raster.y, Color32(0, 0, 0), 1);
			svg.DrawLine(v2Raster.x, screenHeight - v2Raster.y, v0Raster.x, screenHeight - v0Raster.y, Color32(0, 0, 0), 1);
		}
	}
	svg.Save("./test_Rasterization.svg");

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
	delete[] frameBuffer;
	delete[] depthBuffer;
}

void Test_03_SmoothSphere_Diffuse()
{
	Color *frameBuffer = new Color[screenWidth * screenHeight];
	float *depthBuffer = new float[screenWidth * screenHeight];

	for (int i = 0; i < screenWidth * screenHeight; i++)
	{
		// clear frame buffer
		frameBuffer[i].r = 49 / 255.0f;
		frameBuffer[i].g = 77 / 255.0f;
		frameBuffer[i].b = 121 / 255.0f;
		// clear z buffer
		depthBuffer[i] = farClipping;
	}

	// setup lights
	Light light;
	light.color = Color(1, 244 / 255.f, 214 / 255.f);
	light.intensity = 1;
	Vector3 L(0.3213938f, 0.7660444f, -0.5566705f); // light direction

	Color ambient(0, 0, 0); // ambient color

	// setup game objects
	GameObject* object;
	std::vector<GameObject*> gameObjects;

	object = new GameObject();
	object->mesh = ObjLoader::Load("plane_1.obj");
	object->material = new Material();
	object->material->albedo = Color(19 / 255.f, 1, 0);
	gameObjects.push_back(object);

	object = new GameObject();
	object->mesh = ObjLoader::Load("ico-sphere_1-smooth.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	gameObjects.push_back(object);

	object->mesh->Debug();

	object = new GameObject();
	object->mesh = ObjLoader::Load("special-cube.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	gameObjects.push_back(object);

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

			Vector3 v0Raster = WorldToScreenPoint(v0World);
			Vector3 v1Raster = WorldToScreenPoint(v1World);
			Vector3 v2Raster = WorldToScreenPoint(v2World);

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
						float z = 1.f / (w0 / v0Raster.z + w1 / v1Raster.z + w2 / v2Raster.z);

						int idx = y * screenWidth + x;
						if (depthBuffer[idx] > z)
						{
							depthBuffer[idx] = z;

							// Transforming Normal Vectors
							// https://en.wikibooks.org/wiki/GLSL_Programming/Applying_Matrix_Transformations

							const Vector3& N0 = mesh->normals[i0];
							const Vector3& N1 = mesh->normals[i1];
							const Vector3& N2 = mesh->normals[i2];

							Vector3 N = N0 * w0 / v0Raster.z + N1 * w1 / v1Raster.z + N2 * w2 / v2Raster.z;
							N = N / (w0 / v0Raster.z + w1 / v1Raster.z + w2 / v2Raster.z);

							// Remember that an interpolated normal is typically not normalized?
							N = N.normalized();

							Color diffuse = material->albedo * (light.color * light.intensity) * std::max(0.f, Vector3::Dot(N, L));
							frameBuffer[idx] = diffuse + ambient;
						}
					}
				}
			}
		}
	}

	OutputBitmap(frameBuffer, "Test_03_SmoothSphere_Diffuse_ladybug.bmp");

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
	delete[] frameBuffer;
	delete[] depthBuffer;
}

void Test_04_NormalInterpolation()
{
	Color *frameBuffer = new Color[Screen::width * Screen::height];
	float *depthBuffer = new float[Screen::width * Screen::height];

	for (int i = 0; i < Screen::width * Screen::height; i++)
	{
		// clear frame buffer
		frameBuffer[i].r = 49 / 255.0f;
		frameBuffer[i].g = 77 / 255.0f;
		frameBuffer[i].b = 121 / 255.0f;
		// clear z buffer
		depthBuffer[i] = farClipping;
	}

	// setup lights
	Light light;
	light.color = Color(1, 244 / 255.f, 214 / 255.f);
	light.intensity = 1;
	Vector3 L(0.3213938f, 0.7660444f, -0.5566705f); // light direction

	Color ambient(0, 0, 0); // ambient color

	// setup camera
	Camera* camera = new Camera();

	Transform* t = camera->transform;
	t->localPosition = Vector3(7.48113f, 5.34367f, -6.50764f);
	t->localEulerAngles = Vector3(28.321f, -48.981f, 0);
	t->localScale = Vector3(1, 1, 1);

	camera->fieldOfView = 45;
	camera->nearClipPlane = 0.3f;
	camera->farClipPlane = 1000;

	Matrix4x4 modelMatrix = Matrix4x4::identity;
	Matrix4x4 viewMatrix = camera->worldToCameraMatrix();
	Matrix4x4 projectionMatrix = camera->projectionMatrix();

	printf("View Matrix is:\n");
	test_PrintMatrix(viewMatrix);
	printf("Projection Matrix is:\n");
	test_PrintMatrix(projectionMatrix);

	// setup game objects
	GameObject* object;
	std::vector<GameObject*> gameObjects;

	object = new GameObject();
	object->mesh = ObjLoader::Load("plane_1.obj");
	object->material = new Material();
	object->material->albedo = Color(19 / 255.f, 1, 0);
	gameObjects.push_back(object);

	object = new GameObject();
	object->mesh = ObjLoader::Load("ico-sphere_1-smooth.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	gameObjects.push_back(object);

	object = new GameObject();
	object->mesh = ObjLoader::Load("special-cube.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	gameObjects.push_back(object);

	int frameIdx = 0;
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
						float z = 1.f / (w0 / v0Raster.z + w1 / v1Raster.z + w2 / v2Raster.z);

						int idx = y * screenWidth + x;
						if (depthBuffer[idx] > z)
						{
							depthBuffer[idx] = z;

							// Transforming Normal Vectors
							// https://en.wikibooks.org/wiki/GLSL_Programming/Applying_Matrix_Transformations

							const Vector3& N0 = mesh->normals[i0];
							const Vector3& N1 = mesh->normals[i1];
							const Vector3& N2 = mesh->normals[i2];

							Vector3 N = N0 * w0 / v0Raster.z + N1 * w1 / v1Raster.z + N2 * w2 / v2Raster.z;
							N = N / (w0 / v0Raster.z + w1 / v1Raster.z + w2 / v2Raster.z);

							// Remember that an interpolated normal is typically not normalized?
							N = N.normalized();

							Color diffuse = material->albedo * (light.color * light.intensity) * std::max(0.f, Vector3::Dot(N, L));
							frameBuffer[idx] = Color((N.x + 1) / 2, (N.y + 1) / 2, (N.z + 1) / 2, 1);
						}
					}
				}
			}
		}
	}

	OutputBitmap(frameBuffer, "Test_04_NormalInterpolation_ladybug.bmp");

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
	delete[] frameBuffer;
	delete[] depthBuffer;
}

void Test_06_Specular()
{
	Color *frameBuffer = new Color[screenWidth * screenHeight];
	float *depthBuffer = new float[screenWidth * screenHeight];

	for (int i = 0; i < screenWidth * screenHeight; i++)
	{
		// clear frame buffer
		frameBuffer[i].r = 49 / 255.0f;
		frameBuffer[i].g = 77 / 255.0f;
		frameBuffer[i].b = 121 / 255.0f;
		// clear z buffer
		depthBuffer[i] = farClipping;
	}

	// setup lights
	Light light;
	light.color = Color(1, 244 / 255.f, 214 / 255.f);
	light.intensity = 1;
	Vector3 L(0.3213938f, 0.7660444f, -0.5566705f); // light direction

	Color ambient(0, 0, 0); // ambient color
	Color lightColor = light.color * light.intensity;

	// setup camera
	Camera* camera = new Camera();

	Transform* t = camera->transform;
	t->localPosition = Vector3(7.48113f, 5.34367f, -6.50764f);
	t->localEulerAngles = Vector3(28.321f, -48.981f, 0);
	t->localScale = Vector3(1, 1, 1);

	camera->fieldOfView = 45;
	camera->nearClipPlane = 0.3f;
	camera->farClipPlane = 1000;

	// setup game objects
	GameObject* object;
	std::vector<GameObject*> gameObjects;

	object = new GameObject();
	object->mesh = ObjLoader::Load("sphere_1.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	object->material->specular = Color(1, 1, 1);
	object->material->specularGloss = 20;
	gameObjects.push_back(object);

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

			Vector3 v0Raster = WorldToScreenPoint(v0World);
			Vector3 v1Raster = WorldToScreenPoint(v1World);
			Vector3 v2Raster = WorldToScreenPoint(v2World);

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
						float z = 1.f / (w0 / v0Raster.z + w1 / v1Raster.z + w2 / v2Raster.z);

						int idx = y * screenWidth + x;
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

	OutputBitmap(frameBuffer, "Test_06_Specular_ladybug.bmp");

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
	delete[] frameBuffer;
	delete[] depthBuffer;
}

void Test_06_Specular_World()
{
	World world;

	// setup camera
	world.camera = new Camera();

	Transform* t = world.camera->transform;
	t->localPosition = Vector3(7.48113f, 5.34367f, -6.50764f);
	t->localEulerAngles = Vector3(28.321f, -48.981f, 0);

	world.camera->fieldOfView = 45;
	world.camera->nearClipPlane = 0.3f;
	world.camera->farClipPlane = 1000;

	// setup lights
	world.light = new Light();
	world.light->color = Color(1, 244 / 255.f, 214 / 255.f);
	world.light->intensity = 1;

	t = world.light->transform;
	t->localPosition = Vector3(0, 3, 0);
	t->localEulerAngles = Vector3(50, -30, 0);

	// setup game objects
	GameObject* object;

	object = new GameObject();
	object->mesh = ObjLoader::Load("sphere_1.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	object->material->specular = Color(1, 1, 1);
	object->material->specularGloss = 20;

	world.gameObjects.push_back(object);

	world.Render();
	OutputBitmap(world.frameBuffer, "Test_06_Specular_World_ladybug.bmp");
}


void Test_07_ShadowMaps()
{
	Screen::width = 512;
	Screen::height = 512;

	nearClipping = 0.3f;
	farClipping = 20;

	float *shadowMapBuffer = new float[Screen::width * Screen::height];

	for (int i = 0; i < Screen::width * Screen::height; i++)
	{
		// clear z buffer
		shadowMapBuffer[i] = farClipping;
	}

	// setup lights
	Light light;
	light.transform->localPosition = Vector3(-2.6f, 4.28f, -4.5f);
	light.transform->localEulerAngles = Vector3(50, 30, 0);

	light.color = Color(1, 244 / 255.f, 214 / 255.f);
	light.intensity = 1;

	Vector3 L(-0.3213939f, 0.7660444f, -0.5566704f); // light direction

	Color ambient(0, 0, 0); // ambient color
	Color lightColor = light.color * light.intensity;

	// setup camera
	//   orthographic projection
	//   size: 7
	//   near clipping: 0.3
	//   far clipping: 20

	Camera* orthoCamera = new Camera();
	Transform* t = orthoCamera->transform;
	t->localPosition = Vector3(-2.6f, 4.28f, -4.5f);
	t->localEulerAngles = Vector3(50, 30, 0);

	orthoCamera->orthographic = true;
	orthoCamera->orthographicSize = 7;
	orthoCamera->nearClipPlane = 0.3f;
	orthoCamera->farClipPlane = 20;

	// setup game objects
	GameObject* object;
	std::vector<GameObject*> gameObjects;

	object = new GameObject();
	object->mesh = ObjLoader::Load("sphere_1.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	object->material->specular = Color(1, 1, 1);
	object->material->specularGloss = 20;
	gameObjects.push_back(object);

	object = new GameObject();
	object->mesh = ObjLoader::Load("ground_1.obj");
	object->material = new Material();
	object->material->albedo = Color(1, 1, 1);
	object->material->specular = Color(0, 0, 0);
	object->material->specularGloss = 20;
	gameObjects.push_back(object);

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		Mesh* mesh = gameObjects[i]->mesh;
		size_t numTris = mesh->triangles.size() / 3;

		for (size_t idx = 0; idx < numTris; ++idx)
		{
			int i0 = mesh->triangles[idx * 3];
			int i1 = mesh->triangles[idx * 3 + 1];
			int i2 = mesh->triangles[idx * 3 + 2];

			const Vector3& v0World = mesh->vertices[i0];
			const Vector3& v1World = mesh->vertices[i1];
			const Vector3& v2World = mesh->vertices[i2];

			Vector3 v0Raster = orthoCamera->WorldToScreenPoint(v0World);
			Vector3 v1Raster = orthoCamera->WorldToScreenPoint(v1World);
			Vector3 v2Raster = orthoCamera->WorldToScreenPoint(v2World);

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
						float z = w0 * z0 + w1 *z1 + w2 *z2;

						int idx = y * screenWidth + x;
						if (shadowMapBuffer[idx] > z)
						{
							shadowMapBuffer[idx] = z;
						}
					}
				}
			}
		}
	}

	OutputDepthBuffer(shadowMapBuffer, nearClipping, farClipping, "Test_07_ShadowMaps_0_ladybug.bmp");

	// Test ShadowMap class
	{
		World world;
		world.gameObjects = gameObjects;

		ShadowMap shadowMap(&light);
		shadowMap.Render(world);

		OutputDepthBuffer(shadowMap.depthBuffer, nearClipping, farClipping, "Test_07_ShadowMaps_1_ladybug.bmp");
		world.gameObjects = std::vector<GameObject*>();
	}

	Color *frameBuffer = new Color[screenWidth * screenHeight];
	float *depthBuffer = new float[screenWidth * screenHeight];

	for (int i = 0; i < screenWidth * screenHeight; i++)
	{
		// clear frame buffer
		frameBuffer[i].r = 49 / 255.0f;
		frameBuffer[i].g = 77 / 255.0f;
		frameBuffer[i].b = 121 / 255.0f;
		// clear z buffer
		depthBuffer[i] = farClipping;
	}

	// setup camera
	Camera* camera = new Camera();

	t = camera->transform;
	t->localPosition = Vector3(7.48113f, 5.34367f, -6.50764f);
	t->localEulerAngles = Vector3(28.321f, -48.981f, 0);
	t->localScale = Vector3(1, 1, 1);

	camera->fieldOfView = 45;
	camera->nearClipPlane = 0.3f;
	camera->farClipPlane = 1000;

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
						float z = 1.f / (w0 / v0Raster.z + w1 / v1Raster.z + w2 / v2Raster.z);

						int idx = y * screenWidth + x;
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
							Vector3 vLightSpace = orthoCamera->WorldToScreenPoint(vWorld);
							int shadowBufferIdx = (int)(std::roundf(vLightSpace.y) * screenWidth + std::roundf(vLightSpace.x));

							Vector3 reflectDir = reflect(-L, N).normalized();
							Vector3 viewDir = (camera->transform->localPosition - vWorld).normalized();

							Color diffuse = material->albedo * lightColor * std::max(0.f, Vector3::Dot(N, L));
							Color specular = material->specular * lightColor * pow(saturate(Vector3::Dot(reflectDir, viewDir)), material->specularGloss);

							float shadowBias = 0.05f;
							if (vLightSpace.z - shadowBias <= shadowMapBuffer[shadowBufferIdx])
							{
								frameBuffer[idx] = ambient + diffuse + specular;
							}
							else
							{
								frameBuffer[idx] = ambient;
							}
						}
					}
				}
			}
		}
	}

	OutputBitmap(frameBuffer, "Test_07_ShadowMaps_2_no-pcf_bias-0.05_ladybug.bmp");

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
	delete[] shadowMapBuffer;
}