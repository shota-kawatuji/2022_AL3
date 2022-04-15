#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	// X,Y,Z 方向のスケーリングの初期化
	for (int i = 0; i < 20; i++) {
		worldTransfrom_[i].scale_ = {5.0f, 5.0f, 5.0f};
	}

	// X,Y,Z 軸周りの回転角を設定
	for (int i = 0; i < 20; i++) {
		worldTransfrom_[i].rotation_ = {0.0f, 0.0f, 0.0f};
	}

	// X,Y,Z 軸周りの平行移動を設定
	worldTransfrom_[0].translation_ = {-40.0f, 20.0f, 0.0f};
	worldTransfrom_[1].translation_ = {-30.0f, 20.0f, 0.0f};
	worldTransfrom_[2].translation_ = {-20.0f, 20.0f, 0.0f};
	worldTransfrom_[3].translation_ = {-10.0f, 20.0f, 0.0f};
	worldTransfrom_[4].translation_ = {0.0f, 20.0f, 0.0f};
	worldTransfrom_[5].translation_ = {10.0f, 20.0f, 0.0f};
	worldTransfrom_[6].translation_ = {20.0f, 20.0f, 0.0f};
	worldTransfrom_[7].translation_ = {30.0f, 20.0f, 0.0f};
	worldTransfrom_[8].translation_ = {40.0f, 20.0f, 0.0f};
	worldTransfrom_[9].translation_ = {50.0f, 20.0f, 0.0f};

	worldTransfrom_[10].translation_ = {-40.0f, -20.0f, 0.0f};
	worldTransfrom_[11].translation_ = {-30.0f, -20.0f, 0.0f};
	worldTransfrom_[12].translation_ = {-20.0f, -20.0f, 0.0f};
	worldTransfrom_[13].translation_ = {-10.0f, -20.0f, 0.0f};
	worldTransfrom_[14].translation_ = {0.0f, -20.0f, 0.0f};
	worldTransfrom_[15].translation_ = {10.0f, -20.0f, 0.0f};
	worldTransfrom_[16].translation_ = {20.0f, -20.0f, 0.0f};
	worldTransfrom_[17].translation_ = {30.0f, -20.0f, 0.0f};
	worldTransfrom_[18].translation_ = {40.0f, -20.0f, 0.0f};
	worldTransfrom_[19].translation_ = {50.0f, -20.0f, 0.0f};

	for (int i = 0; i < 20; i++) {
		worldTransfrom_[i].Initialize();
	}

	viewProjection_.Initialize();
}


void GameScene::Update() {}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 0; i < 20; i++) {
		model_->Draw(worldTransfrom_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
