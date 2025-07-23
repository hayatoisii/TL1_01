#pragma once
#include "KamataEngine.h"
#include <map>
#include <string>
#include <vector>

using namespace KamataEngine;

class GameScene {

	// オブジェクト 1個分のデータ
	struct ObjectData {
		std::string type; // "type"
		std::string name; // "name"

		// "transform"
		struct Transform {
			Vector3 translation; // "translation"
			Vector3 rotation;    // "rotation"
			Vector3 scaling;     // "scaling"
		};

		Transform transform; // メンバの準備

		// "file_name"
		std::string file_name;
	};

	// レベルデータ
	struct LevelData {
		// "name"
		std::string name;

		// "objects"
		std::vector<ObjectData> objects;
	};

public:
	~GameScene();

	void Initialize();

	void Update();

	void Draw();

private:
	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = nullptr;

	// モデルデータコンテナ
	std::map<std::string, Model*> models;

	// オブジェクトデータ
	std::vector<WorldTransform*> worldTransforms;

	Camera* camera;
};