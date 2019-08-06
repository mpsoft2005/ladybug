
#include <stdio.h>
#include <stdlib.h>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "Test.h"
#include "ObjLoader.h"
#include "Rasterizer.h"
#include "Raytracing.h"

int main()
{
	printf("Welcome to Ladybug Engine!\n");

	Test_06_Specular();
	Test_06_Specular_World();
	Test_07_ShadowMaps();

	return 0;
}
