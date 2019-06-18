#pragma once

#include "Mesh.h"

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	static Mesh* Load(const char* filename);
};

