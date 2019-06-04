
#include <stdio.h>
#include <stdlib.h>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

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
	printf("%.5f %.5f %.5f %.5f\n", transformationMatrix.m00, transformationMatrix.m01, transformationMatrix.m02, transformationMatrix.m03);
	printf("%.5f %.5f %.5f %.5f\n", transformationMatrix.m10, transformationMatrix.m11, transformationMatrix.m12, transformationMatrix.m13);
	printf("%.5f %.5f %.5f %.5f\n", transformationMatrix.m20, transformationMatrix.m21, transformationMatrix.m22, transformationMatrix.m23);
	printf("%.5f %.5f %.5f %.5f\n", transformationMatrix.m30, transformationMatrix.m31, transformationMatrix.m32, transformationMatrix.m33);
}

int main()
{
	printf("Welcome to Ladybug Engine!\n");

	test_Matrix4x4();

	return 0;
}
