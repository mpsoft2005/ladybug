
#include <stdio.h>
#include <stdlib.h>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "Test.h"
#include "ObjLoader.h"
#include "Rasterizer.h"
#include "Raytracing.h"

void setup_models_base_dir(const char* dir)
{
	size_t len = strlen(dir);
	if (dir[len - 1] == '\\' || dir[len - 1] == '/')
	{
		ObjLoader::base_dir = dir;
	}
	else
	{
		ObjLoader::base_dir = dir;
		ObjLoader::base_dir += '\\';
	}
}

int main(int argc, char* argv[])
{
	printf("Welcome to Ladybug Engine!\n");

	if (argc < 2)
	{
		printf("usage:\n");
		printf("  %s <models directory>\n", argv[0]);
		return -1;
	}

	setup_models_base_dir(argv[1]);

	Test_06_Specular();
	Test_06_Specular_World();
	Test_07_ShadowMaps();

	return 0;
}
