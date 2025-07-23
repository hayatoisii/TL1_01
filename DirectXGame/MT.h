#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class MT {
public:
	static Matrix4x4 Matrix4Transform(const Vector3& scale, const Vector3& rotation, const Vector3& translation);
};