#include <stdio.h>

#include "Test.h"

#include "Vector4.h"
#include "Matrix4x4.h"


void test_PrintMatrix(const Matrix4x4& M)
{
	printf("  %.5f %.5f %.5f %.5f\n", M.m00, M.m01, M.m02, M.m03);
	printf("  %.5f %.5f %.5f %.5f\n", M.m10, M.m11, M.m12, M.m13);
	printf("  %.5f %.5f %.5f %.5f\n", M.m20, M.m21, M.m22, M.m23);
	printf("  %.5f %.5f %.5f %.5f\n", M.m30, M.m31, M.m32, M.m33);
}

void test_Matrix4x4()
{
	Matrix4x4 translationMatrix(
		Vector4(1, 0, 0, 0),
		Vector4(0, 1, 0, 0),
		Vector4(0, 0, 1, 0),
		Vector4(1, 2, 3, 1)
	);

	Matrix4x4 rotationMatrix(
		Vector4(0.8434932f, 0.492404f, -0.21461f, 0),
		Vector4(-0.4184121f, 0.8528684f, 0.3123246f, 0),
		Vector4(0.336824f, -0.1736483f, 0.9254165f, 0),
		Vector4(0, 0, 0, 1)
	);

	Matrix4x4 scalingMatrix(
		Vector4(0.1f, 0, 0, 0),
		Vector4(0, 0.2f, 0, 0),
		Vector4(0, 0, 0.3f, 0),
		Vector4(0, 0, 0, 1)
	);

	Matrix4x4 transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;

	printf("Transformation Matrix is:\n");
	test_PrintMatrix(transformationMatrix);
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