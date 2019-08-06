#include "Transform.h"



Transform::Transform()
{
	localPosition = Vector3::zero;
	localEulerAngles = Vector3::zero;
	localScale = Vector3::one;
}


Transform::~Transform()
{
}
