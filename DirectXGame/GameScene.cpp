#include "GameScene.h"
#include "KamataEngine.h"
#include "MT.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <json.hpp>
#include <string>

using namespace KamataEngine;

GameScene::~GameScene() {
	// WorldTransformの解放
	for (auto* wt : worldTransforms) {
		delete wt;
	}
	worldTransforms.clear();

	// Modelの解放
	for (auto& pair : models) {
		delete pair.second;
	}
	models.clear();

	// LevelDataの解放
	delete levelData;
	levelData = nullptr;

	// Cameraの解放
	delete camera;
	camera = nullptr;
}

void GameScene::Initialize() {
	const std::string fullpath = std::string("Resources/levels/") + "scene.json";
	std::ifstream file;
	file.open(fullpath);
	if (file.fail()) {
		assert(0);
	}

	nlohmann::json deserialized;
	file >> deserialized;

	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	levelData = new LevelData();
	levelData->name = deserialized["name"].get<std::string>();
	assert(levelData->name == "scene");

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		// オブジェクト　1つ分の妥当性のチェック
		assert(object.contains("type"));

		if (object["type"].get<std::string>() == "MESH") {
			// 1個分の要素の準備
			levelData->objects.emplace_back(ObjectData{});
			ObjectData& objectData = levelData->objects.back();

			objectData.type = object["type"].get<std::string>(); // "type"
			objectData.name = object["name"].get<std::string>(); // "name"

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動 "translation"
			objectData.transform.translation.x = (float)transform["translation"][0];
			objectData.transform.translation.y = (float)transform["translation"][1];
			objectData.transform.translation.z = (float)transform["translation"][2];
			// 回転 "rotation"
			const float deg2rad = 3.14159265358979323846f / 180.0f;
			objectData.transform.rotation.x = -(float)transform["rotation"][0] * deg2rad;
			objectData.transform.rotation.y = -(float)transform["rotation"][1] * deg2rad;
			objectData.transform.rotation.z = -(float)transform["rotation"][2] * deg2rad;
			// 拡大縮小　"scaling"
			objectData.transform.scaling.x = (float)transform["scaling"][0];
			objectData.transform.scaling.y = (float)transform["scaling"][1];
			objectData.transform.scaling.z = (float)transform["scaling"][2];

			// "file_name"
			if (object.contains("file_name")) {
				objectData.file_name = object["file_name"].get<std::string>();
			}
		}
	}

	// レベルデータに出現するモデルの読み込み
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		decltype(models)::iterator it = models.find(objectData.file_name);

		// 未読み込みの場合、読み込む
		if (it == models.end() && !objectData.file_name.empty()) {
			Model* model = Model::CreateFromOBJ(objectData.file_name);
			models[objectData.file_name] = model;
		}
	}

	// 3Dオブジェクトを生成
	for (auto& objectData : levelData->objects) {
		WorldTransform* worldTransform = new WorldTransform();
		// 座標
		worldTransform->translation_ = objectData.transform.translation;
		// 回転角
		worldTransform->rotation_ = objectData.transform.rotation;
		// 座標
		worldTransform->scale_ = objectData.transform.scaling;

		worldTransform->Initialize();

		// 配列に登録
		worldTransforms.push_back(worldTransform);
	}

	camera = new Camera();
	camera->Initialize();
}

void GameScene::Update() {
	for (WorldTransform* worldTransform : worldTransforms) {
		// Todo::ワールド行列の計算
		worldTransform->matWorld_ = MT::Matrix4Transform(worldTransform->scale_, worldTransform->rotation_, worldTransform->translation_);

		worldTransform->TransferMatrix();
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	int i = 0;

	for (auto& objectData : levelData->objects) {
		// モデルファイル名
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.file_name);
		if (it != models.end()) {
			model = it->second;
		}

		if (model) {
			model->Draw(*worldTransforms[i], *camera);
		}

		i++;
	}

	Model::PostDraw();
}