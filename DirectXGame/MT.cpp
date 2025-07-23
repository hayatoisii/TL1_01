#include "MT.h"
#include <cmath>

using namespace KamataEngine;

// スケール・回転・平行移動からワールド行列を生成
Matrix4x4 MT::Matrix4Transform(const Vector3& scale, const Vector3& rotation, const Vector3& translation) {
	// スケール行列
	Matrix4x4 matScale = {scale.x, 0.0f, 0.0f, 0.0f, 0.0f, scale.y, 0.0f, 0.0f, 0.0f, 0.0f, scale.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	// 回転行列（Z→Y→Xの順で合成）
	float cx = std::cos(rotation.x);
	float sx = std::sin(rotation.x);
	float cy = std::cos(rotation.y);
	float sy = std::sin(rotation.y);
	float cz = std::cos(rotation.z);
	float sz = std::sin(rotation.z);

	Matrix4x4 matRotX = {1, 0, 0, 0, 0, cx, sx, 0, 0, -sx, cx, 0, 0, 0, 0, 1};
	Matrix4x4 matRotY = {cy, 0, -sy, 0, 0, 1, 0, 0, sy, 0, cy, 0, 0, 0, 0, 1};
	Matrix4x4 matRotZ = {cz, sz, 0, 0, -sz, cz, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	// 回転合成（Z→Y→X）
	Matrix4x4 matRot = matRotZ * matRotY * matRotX;

	// スケール・回転合成
	Matrix4x4 matSR = matScale * matRot;

	// 平行移動行列
	Matrix4x4 matTrans = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, translation.x, translation.y, translation.z, 1};

	// 最終ワールド行列
	Matrix4x4 matWorld = matSR * matTrans;
	return matWorld;
}