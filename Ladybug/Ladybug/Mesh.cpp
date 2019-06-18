#include "Mesh.h"



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::Debug()
{
	int numTris = (int)triangles.size() / 3;
	for (int i = 0; i < numTris; i++)
	{
		printf("tri ");
		for (int k = 0; k < 3; k++)
		{
			int idx = triangles[i * 3 + k];
			const Vector3& v = vertices[idx];
			printf("(%f %f %f)", v.x, v.y, v.z);

			if (idx < (int)uv.size())
			{
				const Vector2& vt = uv[idx];
				printf("/(%f %f)", vt.x, vt.y);
			}

			if (idx < (int)normals.size())
			{
				const Vector3& vn = normals[idx];
				printf("/(%f %f %f) ", vn.x, vn.y, vn.z);
			}
		}
		printf("\n");
	}
}
