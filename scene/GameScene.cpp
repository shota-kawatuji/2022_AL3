#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

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

	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスター
	std::mt19937_64 engin(seed_gen());
	// 乱数範囲（回転角）
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	// 乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldTransfrom_); i++) {
		// X,Y,Z 方向のスケーリングを設定
		worldTransfrom_[i].scale_ = {1.0f, 1.0f, 1.0f};
		// X,Y,Z　軸周りの回転角を設定
		//worldTransfrom_[i].rotation_ = {rotDist(engin), rotDist(engin), rotDist(engin)};
		worldTransfrom_[i].rotation_ = {0, 0, 0};
		// X,Y,Z　軸周りの平行移動を設定
		//worldTransfrom_[i].translation_ = {posDist(engin), posDist(engin), posDist(engin)};
		worldTransfrom_[i].translation_ = {0, 0, 0};

		// ワールドトランスフォームの初期化
		worldTransfrom_[i].Initialize();
	}
	// カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = XMConvertToRadians(45.0f);

	//// アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;
	//// ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	//// ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

	// キャラクターの大元
	worldTransfrom_[PartId::Root].Initialize();
	// 脊椎
	worldTransfrom_[PartId::Spine].translation_ = {0, 0, 0};
	worldTransfrom_[PartId::Spine].parent_ = &worldTransfrom_[PartId::Root];
	worldTransfrom_[PartId::Spine].Initialize();
	// 上半身
	worldTransfrom_[PartId::Chest].Initialize();
	worldTransfrom_[PartId::Chest].parent_ = &worldTransfrom_[PartId::Spine];
	worldTransfrom_[PartId::Chest].translation_ = {0, 2.5f, 0};

	worldTransfrom_[PartId::Head].Initialize();
	worldTransfrom_[PartId::Head].parent_ = &worldTransfrom_[PartId::Chest];
	worldTransfrom_[PartId::Head].translation_ = {0, 2.5f, 0};

	worldTransfrom_[PartId::ArmL].Initialize();
	worldTransfrom_[PartId::ArmL].parent_ = &worldTransfrom_[PartId::Chest];
	worldTransfrom_[PartId::ArmL].translation_ = {-2.5, 0, 0};

	worldTransfrom_[PartId::ArmR].Initialize();
	worldTransfrom_[PartId::ArmR].parent_ = &worldTransfrom_[PartId::Chest];
	worldTransfrom_[PartId::ArmR].translation_ = {2.5, 0, 0};
	// 下半身
	worldTransfrom_[PartId::Hip].Initialize();
	worldTransfrom_[PartId::Hip].parent_ = &worldTransfrom_[PartId::Spine];
	worldTransfrom_[PartId::Hip].translation_ = {0, 0, 0};

	worldTransfrom_[PartId::LegL].Initialize();
	worldTransfrom_[PartId::LegL].parent_ = &worldTransfrom_[PartId::Hip];
	worldTransfrom_[PartId::LegL].translation_ = {-2.5f, -2.5f, 0};

	worldTransfrom_[PartId::LegR].Initialize();
	worldTransfrom_[PartId::LegR].parent_ = &worldTransfrom_[PartId::Hip];
	worldTransfrom_[PartId::LegR].translation_ = {2.5, -2.5f, 0};


	viewProjection_.Initialize();
}


void GameScene::Update() {
	#pragma region 02

	//// 視点移動処理
	//// 視点の移動ベクトル
	//XMFLOAT3 move = {0, 0, 0};

	//// 視点の移動速さ
	//const float kEyeSpeed = 0.2f;

	//// 押した移動の速さ
	//if (input_->PushKey(DIK_W)) {
	//	move = {0, 0, kEyeSpeed};
	//} else if (input_->PushKey(DIK_S)) {
	//	move = {0, 0, -kEyeSpeed};
	//}

	//// 視点移動（ベクトルの加算）
	//viewProjection_.eye.x += move.x;
	//viewProjection_.eye.y += move.y;
	//viewProjection_.eye.z += move.z;

	//// 行列の再計算
	//viewProjection_.UpdateMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	//// 注視点移動処理
	////// 注視点の移動ベクトル
	////XMFLOAT3 move = {0, 0, 0};

	//// 注視点の移動速さ
	//const float kTargetSpeed = 0.2f;

	//// 押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_LEFT)) {
	//	move = {-kTargetSpeed, 0, 0};
	//} else if (input_->PushKey(DIK_RIGHT)) {
	//	move = {kTargetSpeed, 0, 0};
	//}

	//// 注視点移動（ベクトルの加算）
	//viewProjection_.target.x += move.x;
	//viewProjection_.target.y += move.y;
	//viewProjection_.target.z += move.z;

	//// 行列の再計算
	//viewProjection_.UpdateMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	//// 上方向回転処理
	//// 上方向の回転速さ[ラジアン/frame]
	//const float kUpRotSpeed = 0.05f;

	//// 押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_SPACE)) {
	//	viewAngle += kUpRotSpeed;
	//	// 2πを超えたら0に戻す
	//	viewAngle = fmodf(viewAngle, XM_2PI);
	//}

	//// 上方向ベクトルを計算（半径1の円周上の座標）
	//viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	////行列の再計算
	//viewProjection_.UpdateMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

#pragma endregion

	#pragma region 03

	//// Fol/変更処理
	//// 上キーで視野角が広がる
	//if (input_->PushKey(DIK_UP)) {
	//	viewProjection_.fovAngleY += 0.01f;
	//	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	//// 下キーで視野角が狭まる
	//} else if (input_->PushKey(DIK_DOWN)) {
	//	viewProjection_.fovAngleY -= 0.01f;
	//	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	//}
	//// 行列の再計算
	//viewProjection_.UpdateMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Dgree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));

	//// クリップ距離変更処理
	//// 上下キーでニアクリップ距離を増減
	//if (input_->PushKey(DIK_UP)) {
	//	viewProjection_.nearZ += 0.1f;
	//} else if (input_->PushKey(DIK_DOWN)) {
	//	viewProjection_.nearZ -= 0.1f;
	//}

	//// 行列の再計算
	//viewProjection_.UpdateMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);

#pragma endregion

	// キャラクターの移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	// 注視点移動(ベクトルの加算)
	worldTransfrom_[PartId::Root].translation_.x += move.x;
	worldTransfrom_[PartId::Root].translation_.y += move.y;
	worldTransfrom_[PartId::Root].translation_.z += move.z;

	// デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransfrom_[PartId::Root].translation_.x,
	  worldTransfrom_[PartId::Root].translation_.y, worldTransfrom_[PartId::Root].translation_.z);

	worldTransfrom_[PartId::Root].UpdateMatrix();
	worldTransfrom_[PartId::Spine].UpdateMatrix();
	worldTransfrom_[PartId::Chest].UpdateMatrix();
	worldTransfrom_[PartId::Head].UpdateMatrix();
	worldTransfrom_[PartId::ArmL].UpdateMatrix();
	worldTransfrom_[PartId::ArmR].UpdateMatrix();
	worldTransfrom_[PartId::Hip].UpdateMatrix();
	worldTransfrom_[PartId::LegL].UpdateMatrix();
	worldTransfrom_[PartId::LegR].UpdateMatrix();

	// 上半身回転処理
	// 上半身の回転の速さ[ラジアン/freme]
	const float kChestRotSpeed = 0.05f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_U)) {
		worldTransfrom_[PartId::Chest].rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_I)) {
		worldTransfrom_[PartId::Chest].rotation_.y += kChestRotSpeed;
	}


	// 下半身回転処理
	// 下半身の回転の速さ[ラジアン/freme]
	const float kHipRotSpeed = 0.05f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_J)) {
		worldTransfrom_[PartId::Hip].rotation_.y -= kHipRotSpeed;
	} else if (input_->PushKey(DIK_K)) {
		worldTransfrom_[PartId::Hip].rotation_.y += kHipRotSpeed;
	}

}

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
	//for (size_t i = 0; i < _countof(worldTransfrom_); i++) {
	//	model_->Draw(worldTransfrom_[i], viewProjection_, textureHandle_);
	//}
	// キャラクターの描画
	//model_->Draw(worldTransfrom_[PartId::Root], viewProjection_, textureHandle_);
	//model_->Draw(worldTransfrom_[PartId::Spine], viewProjection_, textureHandle_);
	model_->Draw(worldTransfrom_[PartId::Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransfrom_[PartId::Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransfrom_[PartId::ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransfrom_[PartId::ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransfrom_[PartId::Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransfrom_[PartId::LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransfrom_[PartId::LegR], viewProjection_, textureHandle_);

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
