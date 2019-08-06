#pragma once

#include <memory>
#include "Mesh.h"

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	static std::shared_ptr<Mesh> Load(const char* filename);
};

