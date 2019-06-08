#pragma once

class Color;
class Vector3;
class Matrix4x4;

void test_PrintVector3(const Vector3& v);
void test_PrintMatrix(const Matrix4x4& M);
void test_Matrix4x4();
void test_PerspectiveMatrix();
void test_SavePPM(const char* filename, int width, int height, const Color* buffer);
