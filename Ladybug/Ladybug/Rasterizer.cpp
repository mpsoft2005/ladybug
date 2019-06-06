
#include <stdio.h>
#include <fstream>
#include <algorithm>

#include "Rasterizer.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Test.h"

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



static Vector3 vertices[] {
	Vector3(),
	Vector3(1.000000f, - 1.000000f, 5.000000f),
	Vector3(-1.000000f, - 1.000000f, 5.000000f),
	Vector3(1.000000f, 1.000000f, 5.000000f),
	Vector3(-1.000000f, 1.000000f, 5.000000f)
};

struct Tri
{
public:
	int v0;
	int v1;
	int v2;

public:
	Tri(int v0, int v1, int v2)
	{
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
	}
};

static Tri tris[]{
	Tri(2, 3, 1),
	Tri(2, 4, 3)
};

const int screenWidth = 640;
const int screenHeight = 480;

const float nearClipping = 0.3f;
const float farClipping = 1000.0f;

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



void test_Raster()
{
	printf("MVP Matrix:\n");
	test_PrintMatrix(mvpMatrix);

	Vector3 worldPoint(1.000000f, -1.000000f, 5.000000f);
	printf("World Point:\n");
	test_PrintVector3(worldPoint);

	Vector3 viewportPoint = WorldToViewportPoint(worldPoint);
	printf("Viewport Point:\n");
	test_PrintVector3(viewportPoint);

	Vector3 screenPoint = WorldToScreenPoint(worldPoint);
	printf("Screen Point:\n");
	test_PrintVector3(screenPoint);


	std::ofstream ofs;
	ofs.open("./Plane.svg");
	ofs << "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" width=\"" << screenWidth << "\" height=\"" << screenHeight << "\">" << std::endl;
	ofs << "<rect width=\"" << screenWidth << "\" height=\"" << screenHeight << "\" style=\"fill:rgb(0, 0, 255); stroke - width:3; stroke:rgb(0, 0, 0)\" />";

	for (int i = 0; i < 2; ++i) {
		const Vector3& v0World = vertices[tris[i].v0];
		const Vector3& v1World = vertices[tris[i].v1];
		const Vector3& v2World = vertices[tris[i].v2];

		Vector3 v0Raster = WorldToScreenPoint(v0World);
		Vector3 v1Raster = WorldToScreenPoint(v1World);
		Vector3 v2Raster = WorldToScreenPoint(v2World);

		ofs << "<line x1=\"" << (int)v0Raster.x << "\" y1=\"" << screenHeight - (int)v0Raster.y << "\" x2=\"" << (int)v1Raster.x << "\" y2=\"" << screenHeight - (int)v1Raster.y << "\" style=\"stroke:rgb(0,0,0);stroke-width:1\" />\n";
		ofs << "<line x1=\"" << (int)v1Raster.x << "\" y1=\"" << screenHeight - (int)v1Raster.y << "\" x2=\"" << (int)v2Raster.x << "\" y2=\"" << screenHeight - (int)v2Raster.y << "\" style=\"stroke:rgb(0,0,0);stroke-width:1\" />\n";
		ofs << "<line x1=\"" << (int)v2Raster.x << "\" y1=\"" << screenHeight - (int)v2Raster.y << "\" x2=\"" << (int)v0Raster.x << "\" y2=\"" << screenHeight - (int)v0Raster.y << "\" style=\"stroke:rgb(0,0,0);stroke-width:1\" />\n";
	}


	ofs << "</svg>\n";
	ofs.close();

}

inline float min(float a, float b, float c)
{
	return std::min(a, std::min(b, c));
}

inline float max(float a, float b, float c)
{
	return std::max(a, std::max(b, c));
}

inline float edgeFunction(const Vector3& a, const Vector3& b, const Vector3& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void test_Rasterization()
{
	Vector4 *frameBuffer = new Vector4[screenWidth * screenHeight];
	float *depthBuffer = new float[screenWidth * screenHeight];

	for (int i = 0; i < screenWidth * screenHeight; i++)
	{
		// clear frame buffer
		frameBuffer[i].x = 20 / 255.0f;
		frameBuffer[i].y = 20 / 255.0f;
		frameBuffer[i].z = 20 / 255.0f;
		frameBuffer[i].w = 0;
		// clear z buffer
		depthBuffer[i] = farClipping;
	}

	for (int i = 0; i < 2; ++i)
	{
		const Vector3& v0World = vertices[tris[i].v0];
		const Vector3& v1World = vertices[tris[i].v1];
		const Vector3& v2World = vertices[tris[i].v2];

		Vector3 v0Raster = WorldToScreenPoint(v0World);
		Vector3 v1Raster = WorldToScreenPoint(v1World);
		Vector3 v2Raster = WorldToScreenPoint(v2World);

		float area = edgeFunction(v0Raster, v1Raster, v2Raster);

		float xmin = min(v0Raster.x, v1Raster.x, v2Raster.x);
		float ymin = min(v0Raster.y, v1Raster.y, v2Raster.y);
		float xmax = max(v0Raster.x, v1Raster.x, v2Raster.x);
		float ymax = max(v0Raster.y, v1Raster.y, v2Raster.y);

		int x0 = std::max(0, (int)(std::roundf(xmin)));
		int x1 = std::min(screenWidth - 1, (int)(std::roundf(xmax)));
		int y0 = std::max(0, (int)(std::roundf(ymin)));
		int y1 = std::min(screenHeight - 1, (int)(std::roundf(ymax)));

		for (int y = y0; y <= y1; y++)
		{
			for (int x = x0; x <= x1; x++)
			{
				Vector3 pixelSample(x + 0.5f, y + 0.5f, 0);
				float w0 = edgeFunction(v0Raster, v1Raster, pixelSample);
				float w1 = edgeFunction(v1Raster, v2Raster, pixelSample);
				float w2 = edgeFunction(v2Raster, v0Raster, pixelSample);

				if (w0 >= 0 && w1 >= 0 && w2 >= 0)
				{
					w0 /= area; w1 /= area; w2 /= area;
					float oneOverZ = v0Raster.z * w0 + v1Raster.z * w1 + v2Raster.z * w2;
					float z = 1 / oneOverZ;

					int idx = y * screenWidth + x;
					if (depthBuffer[idx] > z)
					{
						depthBuffer[idx] = z;
						frameBuffer[idx].x = 89 / 255.0f;
						frameBuffer[idx].y = 89 / 255.0f;
						frameBuffer[idx].z = 89 / 255.0f;
						frameBuffer[idx].w = 0;
					}
				}
			}
		}
	}

	std::ofstream ofs;
	ofs.open("./Plane.ppm");
	ofs << "P6\n" << screenWidth << " " << screenHeight << "\n255\n";
	for (int y = screenHeight - 1; y >= 0; y--)
	{
		for (int x = 0; x < screenWidth; x++)
		{
			const Vector4& pixel = frameBuffer[y * screenWidth + x];
			ofs << (uint8_t)(pixel.x * 255) << (uint8_t)(pixel.y * 255) << (uint8_t)(pixel.z * 255);
		}
	}
	ofs.close();

	delete[] frameBuffer;
	delete[] depthBuffer;
}