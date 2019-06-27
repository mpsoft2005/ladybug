#pragma once

#include "Color.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Mesh.h"

Mesh* test_CreatePlaneMesh();
Mesh* test_CreateCubeMesh();

void test_PrintVector3(const Vector3& v);
void test_PrintMatrix(const Matrix4x4& M);
void test_Matrix4x4();
void test_PerspectiveMatrix();
void test_SavePPM(const char* filename, int width, int height, const Color* buffer);
void test_Camera();
