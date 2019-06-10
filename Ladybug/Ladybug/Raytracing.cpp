
#include <math.h>

#include "Raytracing.h"
#include "Ray.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Color.h"
#include "Mesh.h"
#include "Bitmap.h"
#include "Test.h"

const float kEpsilon = 1e-8f;

static inline float Radians(float degree)
{
	return degree * (float)M_PI / 180;
}

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

static Matrix4x4 mvMatrix = viewMatrix * modelMatrix; // object space to camera space

const int screenWidth = 640;
const int screenHeight = 480;

const float fovy = 45; // in degree
const float nearClipping = 0.3f;
const float farClipping = 1000.0f;

const float aspect = (float)screenWidth / screenHeight;
const float tanHalfFovy = tanf(Radians(fovy) / 2);
const float nearClippingPlaneHeight = nearClipping * tanHalfFovy * 2;
const float nearClippingPlaneWidth = nearClippingPlaneHeight * aspect;

struct Triangle
{
	Vector3 v0;
	Vector3 v1;
	Vector3 v2;

	Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2)
	{
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
	}
};

struct RaycastHit
{
	Vector3 point;
	Vector3 normal;
	Vector3 barycentricCoordinate;
	float distance;
	Vector2 textureCoord;
};

bool Raycast(const Ray& ray, const Triangle& tri, RaycastHit &hitInfo)
{
	// calc plane's normal
	Vector3 v0v1 = tri.v1 - tri.v0;
	Vector3 v0v2 = tri.v2 - tri.v0;
	Vector3 N = Vector3::Cross(v0v1, v0v2);
	float denom = Vector3::Dot(N, N);

	float NdotRay = Vector3::Dot(N, ray.direction);
	if (fabs(NdotRay) < kEpsilon) return false; // ray and plane are parallel

	float d = Vector3::Dot(N, tri.v0);
	float t = (Vector3::Dot(N, ray.origin) + d) / NdotRay;
	if (t < 0) return false; // the triangle is behind

	// calc intersection point
	Vector3 P = ray.origin + ray.direction * t;

	// inside-outside test
	Vector3 C; // vector perpendicular to triangle's plane 

	// edge 0
	Vector3 edge0 = tri.v1 - tri.v0;
	Vector3 vp0 = P - tri.v0;
	C = Vector3::Cross(edge0, vp0);
	float w = Vector3::Dot(N, C);
	if (w < 0) return false; // P is on the right side 

	// edge 1
	Vector3 edge1 = tri.v2 - tri.v1;
	Vector3 vp1 = P - tri.v1;
	C = Vector3::Cross(edge1, vp1);
	float u = Vector3::Dot(N, C);
	if (u < 0)  return false; // P is on the right side 

	// edge 2
	Vector3 edge2 = tri.v0 - tri.v2;
	Vector3 vp2 = P - tri.v2;
	C = Vector3::Cross(edge2, vp2);
	float v = Vector3::Dot(N, C);
	if (v < 0) return false; // P is on the right side; 

	hitInfo.point = P;
	hitInfo.distance = t;
	hitInfo.textureCoord.x = u / denom;
	hitInfo.textureCoord.y = v / denom;
	return true;
}

// Returns a ray going from camera through a screen point.
// Note: ray in camera space
Ray ScreenPointToRay(float x, float y)
{
	float viewportX = x / screenWidth;
	float viewportY = y / screenHeight;

	float ndcX = viewportX * 2 - 1;
	float ndcY = viewportY * 2 - 1;

	float nearX = ndcX * nearClippingPlaneWidth / 2;
	float nearY = ndcY * nearClippingPlaneHeight / 2;

	Vector3 direction(nearX, nearY, -nearClipping);
	return Ray(Vector3::zero, direction);
}

void test_PrintRay(const Ray& ray)
{
	printf("ray origin(%f, %f, %f) direction(%f, %f, %f)\n",
		ray.origin.x, ray.origin.y, ray.origin.z,
		ray.direction.x, ray.direction.y, ray.direction.z);
}

void test_Raytracing()
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

	Mesh* mesh = test_CreatePlaneMesh();
	size_t numTris = mesh->triangles.size() / 3;

	for (int y = 0; y < screenHeight; y++) {
		for (int x = 0; x < screenWidth; x++) {
			Ray ray = ScreenPointToRay(x + 0.5f, y + 0.5f);
			RaycastHit hitInfo;
			for (int i = 0; i < numTris; i++) {
				const Vector3& v0 = mesh->vertices[mesh->triangles[i * 3]];
				const Vector3& v1 = mesh->vertices[mesh->triangles[i * 3 + 1]];
				const Vector3& v2 = mesh->vertices[mesh->triangles[i * 3 + 2]];

				Triangle tri(mvMatrix.MultiplyPoint(v0), mvMatrix.MultiplyPoint(v1), mvMatrix.MultiplyPoint(v2));
				if (Raycast(ray, tri, hitInfo)) {
					int idx = y * screenWidth + x;
					if (depthBuffer[idx] > hitInfo.distance)
					{
						depthBuffer[idx] = hitInfo.distance;
						frameBuffer[idx].r = 89 / 255.0f;
						frameBuffer[idx].g = 89 / 255.0f;
						frameBuffer[idx].b = 89 / 255.0f;
					}
				}
			}
		}
	}

	Bitmap bitmap(screenWidth, screenHeight);
	for (int y = 0; y < screenHeight; y++)
	{
		for (int x = 0; x < screenWidth; x++)
		{
			const Color& pixel = frameBuffer[y * screenWidth + x];
			bitmap.SetPixel(x, y, pixel);
		}
	}
	bitmap.Save("./test_Raytracing.bmp");

	delete mesh;
	delete[] depthBuffer;
	delete[] frameBuffer;
}
