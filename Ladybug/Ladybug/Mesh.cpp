#include "Mesh.h"



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::Debug()
{
	for (int i = 0; i < (int)vertices.size(); i++)
	{
		const Vector3& v = vertices[i];
		printf("v %.6f %.6f %.6f\n", v.x, v.y, v.z);
	}

	for (int i = 0; i < (int)uv.size(); i++)
	{
		const Vector2& vt = uv[i];
		printf("vt %.6f %.6f\n", vt.x, vt.y);
	}

	for (int i = 0; i < (int)normals.size(); i++)
	{
		const Vector3& vn = normals[i];
		printf("vn %.6f %.6f %.6f\n", vn.x, vn.y, vn.z);
	}

	int numTris = (int)triangles.size() / 3;
	for (int i = 0; i < numTris; i++)
	{
		printf("f %d %d %d\n", triangles[i * 3], triangles[i * 3 + 1], triangles[i * 3 + 2]);
	}
}

void Mesh::DebugTri()
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
