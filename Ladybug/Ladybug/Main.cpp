
#include <stdio.h>
#include <stdlib.h>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Test.h"

int main()
{
	printf("Welcome to Ladybug Engine!\n");

	test_Matrix4x4();
	test_PerspectiveMatrix();

	return 0;
}
