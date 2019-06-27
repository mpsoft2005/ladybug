
#include <fstream>
#include <stdio.h>

#include "Test.h"

#include "Color.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "GameObject.h"
#include "Camera.h"


Mesh* test_CreatePlaneMesh()
{
	Mesh* mesh = new Mesh();

	mesh->vertices.push_back(Vector3());
	mesh->vertices.push_back(Vector3(1.000000f, -1.000000f, 5.000000f));
	mesh->vertices.push_back(Vector3(-1.000000f, -1.000000f, 5.000000f));
	mesh->vertices.push_back(Vector3(1.000000f, 1.000000f, 5.000000f));
	mesh->vertices.push_back(Vector3(-1.000000f, 1.000000f, 5.000000f));

	mesh->triangles.push_back(2);
	mesh->triangles.push_back(3);
	mesh->triangles.push_back(1);
	mesh->triangles.push_back(2);
	mesh->triangles.push_back(4);
	mesh->triangles.push_back(3);

	return mesh;
}

Mesh* test_CreateCubeMesh()
{
	Mesh* mesh = new Mesh();

	mesh->vertices.push_back(Vector3());
	mesh->vertices.push_back(Vector3(-1.000000f, 0.000000f, 1.000000f));
	mesh->vertices.push_back(Vector3(-1.000000f, 0.000000f, - 1.000000f));
	mesh->vertices.push_back(Vector3(1.000000f, 0.000000f, - 1.000000f));
	mesh->vertices.push_back(Vector3(1.000000f, 0.000000f, 1.000000f));
	mesh->vertices.push_back(Vector3(-1.000000f, 2.000000f, 0.999999f));
	mesh->vertices.push_back(Vector3(-0.999999f, 2.000000f, - 1.000001f));
	mesh->vertices.push_back(Vector3(1.000000f, 2.000000f, - 1.000000f));
	mesh->vertices.push_back(Vector3(1.000000f, 2.000000f, 1.000000f));

	mesh->triangles.push_back(2);
	mesh->triangles.push_back(4);
	mesh->triangles.push_back(1);

	mesh->triangles.push_back(8);
	mesh->triangles.push_back(6);
	mesh->triangles.push_back(5);

	mesh->triangles.push_back(5);
	mesh->triangles.push_back(2);
	mesh->triangles.push_back(1);

	mesh->triangles.push_back(6);
	mesh->triangles.push_back(3);
	mesh->triangles.push_back(2);

	mesh->triangles.push_back(3);
	mesh->triangles.push_back(8);
	mesh->triangles.push_back(4);

	mesh->triangles.push_back(1);
	mesh->triangles.push_back(8);
	mesh->triangles.push_back(5);

	mesh->triangles.push_back(2);
	mesh->triangles.push_back(3);
	mesh->triangles.push_back(4);

	mesh->triangles.push_back(8);
	mesh->triangles.push_back(7);
	mesh->triangles.push_back(6);

	mesh->triangles.push_back(5);
	mesh->triangles.push_back(6);
	mesh->triangles.push_back(2);

	mesh->triangles.push_back(6);
	mesh->triangles.push_back(7);
	mesh->triangles.push_back(3);

	mesh->triangles.push_back(3);
	mesh->triangles.push_back(7);
	mesh->triangles.push_back(8);

	mesh->triangles.push_back(1);
	mesh->triangles.push_back(4);
	mesh->triangles.push_back(8);

	return mesh;
}

void test_PrintVector3(const Vector3& v)
{
	printf("  %.6f %.6f %.6f\n", v.x, v.y, v.z);
}

void test_PrintMatrix(const Matrix4x4& M)
{
	printf("  %.6f %.6f %.6f %.6f\n", M.m00, M.m01, M.m02, M.m03);
	printf("  %.6f %.6f %.6f %.6f\n", M.m10, M.m11, M.m12, M.m13);
	printf("  %.6f %.6f %.6f %.6f\n", M.m20, M.m21, M.m22, M.m23);
	printf("  %.6f %.6f %.6f %.6f\n", M.m30, M.m31, M.m32, M.m33);
}

void test_Matrix4x4()
{
	Matrix4x4 translationMatrix(
		Vector4(1, 0, 0, 0),
		Vector4(0, 1, 0, 0),
		Vector4(0, 0, 1, 0),
		Vector4(1, 2, 3, 1)
	);
	printf("translationMatrix is:\n");
	test_PrintMatrix(translationMatrix);

	Matrix4x4 rotationMatrix(
		Vector4(0.8434932f, 0.492404f, -0.21461f, 0),
		Vector4(-0.4184121f, 0.8528684f, 0.3123246f, 0),
		Vector4(0.336824f, -0.1736483f, 0.9254165f, 0),
		Vector4(0, 0, 0, 1)
	);
	printf("rotationMatrix is:\n");
	test_PrintMatrix(rotationMatrix);

	Matrix4x4 scalingMatrix(
		Vector4(0.1f, 0, 0, 0),
		Vector4(0, 0.2f, 0, 0),
		Vector4(0, 0, 0.3f, 0),
		Vector4(0, 0, 0, 1)
	);
	printf("scalingMatrix is:\n");
	test_PrintMatrix(scalingMatrix);

	Matrix4x4 transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;

	printf("transformationMatrix is:\n");
	test_PrintMatrix(transformationMatrix);

	printf("transformationMatrix.inverse() is:\n");
	test_PrintMatrix(transformationMatrix.inverse());
}

void test_PerspectiveMatrix()
{
	int screenWidth = 640;
	int screenHeight = 480;
	float fovy = 60;
	float near = 0.3f;
	float far = 1000.0f;

	float aspect = (float)screenWidth / screenHeight;
	Matrix4x4 perspectiveMatrix = Matrix4x4::Perspective(fovy, aspect, near, far);

	printf("Perspective Matrix is:\n");
	test_PrintMatrix(perspectiveMatrix);
}

void test_SavePPM(const char* filename, int width, int height, const Color* buffer)
{
	std::ofstream ofs;
	ofs.open("./Plane.ppm");
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (int y = height - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			const Color& pixel = buffer[y * width + x];
			ofs << (uint8_t)(pixel.r * 255) << (uint8_t)(pixel.g * 255) << (uint8_t)(pixel.b * 255);
		}
	}
	ofs.close();
}

void test_Camera()
{
	GameObject* mainCamera = new GameObject();

	Transform* t = mainCamera->transform;
	t->localPosition = Vector3(7.48113f, 5.34367f, -6.50764f);
	t->localEulerAngles = Vector3(28.321f, -48.981f, 0);
	t->localScale = Vector3(1, 1, 1);

	Camera* camera = mainCamera->AddComponent<Camera>();
	Matrix4x4 worldToCamera = camera->worldToCameraMatrix();

	printf("worldToCamera matrix:\n");
	test_PrintMatrix(worldToCamera);

	delete mainCamera;
}
