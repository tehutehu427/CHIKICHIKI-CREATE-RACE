#include "ModeSelect.h"
#include <string>
#include <DxLib.h>
#include <cmath>
#include "../../../Common/FontRegistry.h"
#include "../../../Utility/Utility.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Renderer/PixelMaterial.h"
#include "../../../Renderer/PixelRenderer.h"
#include "../../../Scene/SelectScene.h"
#include "../ManualTab.h"
#include "SelectSceneActors.h"


namespace
{
	Vector2 imgArcSize = { ResourceManager::IMG_ARC_SIZE, ResourceManager::IMG_ARC_SIZE };
	Vector2 imgArcDivs = { ResourceManager::IMG_ARCS_DIV_X, ResourceManager::IMG_ARCS_DIV_Y };
}

ModeSelect::ModeSelect()
	: scnMng_(SceneManager::GetInstance()),
	dateBank_(DateBank::GetInstance())
{
	//状態別更新関数のセット
	RegisterArcState(UPD_STATE::SELECT, [&](SelectScene& _parent) { SelectUpdate(_parent); });
	RegisterArcState(UPD_STATE::ROTATE, [&](SelectScene& _parent) { RotateUpdate(_parent); });

	//変数の初期化
	for (int i = 0; i < DRAW_ARC_NUM; i++) { arc_[i] = {}; }
	int i = -1;
	imgArcs_ = &i;
	imgMessages_ = &i;
	currentAngle_ = 0.0f;
	targetAngle_ = 0.0f;
	arcIndex_ = 0;
	menuIndex_ = 0;
	updState_ = UPD_STATE::NONE;
	material_ = nullptr;
	renderer_ = nullptr;
	imgBackArc_ = 0;
	imgShadowArc_ = 0;
	imgArrow_ = 0; 
	manual_ = nullptr;
	actors_ = nullptr;
}

ModeSelect::~ModeSelect()
{
}

void ModeSelect::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();

	//リソースの読み込み
	imgMessages_ = res.Load(ResourceManager::SRC::SELECT_MESSAGES).handleIds_;
	imgArcs_ = res.Load(ResourceManager::SRC::ARCS).handleIds_;
	imgBackArc_ = res.Load(ResourceManager::SRC::BACK_ARC).handleId_;
	imgShadowArc_ = res.Load(ResourceManager::SRC::SHADOW_ARC).handleId_;
	imgArrow_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;

	//描画の設定
	material_ = std::make_unique<PixelMaterial>("RotateGlow.cso", 2);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	manual_ = std::make_unique<ManualTab>();
	manual_->Load();

	actors_ = std::make_unique<SelectSceneActors>();
	actors_->Load();
}

void ModeSelect::Init()
{
	//初期化
	//画像分割り当て
	for (int i = 0; i < DRAW_ARC_NUM; i++)
	{
		//オフセット角：等間隔
		arc_[i].angle = currentAngle_ + ROTATE_STEP * i;

		// 位置計算
		arc_[i].pos.x = static_cast<int>(ROTATE_CENTER_X + std::cos(arc_[i].angle) * ORBIT_RADIUS);
		arc_[i].pos.y = static_cast<int>(ROTATE_CENTER_Y + std::sin(arc_[i].angle) * ORBIT_RADIUS);

		//画像のハンドル
		arc_[i].img = imgArcs_[i];
	}
	//最後の円弧の要素だけメニュー項目の最後の要素にする
	arc_[DRAW_ARC_NUM - 1].img = imgArcs_[SelectScene::MENU_LIST_NUM - 1];

	//選択メニューの初期化
	ChangeUpdateState(UPD_STATE::SELECT);

	//ターゲット角度を初期化
	targetAngle_ = 0.0f;

	//マニュアル初期化
	manual_->Init();

	//バッファーの設定
	material_->AddConstBuf(FLOAT4{ 1.0f,0.8f,0.0f,0.7f });
	material_->AddConstBuf(FLOAT4{ 
		ResourceManager::IMG_ARC_SIZE,
		ResourceManager::IMG_ARC_SIZE,
		arc_[arcIndex_].angle,
		BLUR_DISTANCE });

	//テクスチャ設定
	material_->AddTextureBuf(imgBackArc_);

	//座標サイズ設定
	renderer_->SetPos(arc_[arcIndex_].pos);
	renderer_->SetSize({ ResourceManager::IMG_ARC_SIZE, ResourceManager::IMG_ARC_SIZE });

	//アクター初期化
	actors_->Init();

}

void ModeSelect::Update(SelectScene& _parent)
{
#ifdef _DEBUG
	DebugUpdate();
#endif

	// 毎フレームのUpdate処理
	if (selectUpdateFunc_ && !manual_->IsDisplay()) selectUpdateFunc_(_parent);

	//マニュアル更新
	manual_->Update();

	//アクター更新
	actors_->Update();
}

void ModeSelect::Draw()
{
#ifdef _DEBUG
	DebugDraw();
#endif

	//アクターの描画
	actors_->Draw();

	//矢印の描画
	DrawArrow();

	//メッセージの描画
	DrawMessage();

	//UIの描画
	for (int i = 0; i < DRAW_ARC_NUM; i++)
	{
		if (i == arcIndex_)
		{
			
			DrawGlow(arcIndex_);
		}
		else
		{
			DrawDarkly(i);
		}
	}
	//マニュアル描画
	manual_->Draw();
}

void ModeSelect::RegisterArcState(const UPD_STATE _state, std::function<void(SelectScene&)> _update)
{
	stateMap_[_state] = _update;
}

void ModeSelect::ChangeUpdateState(const UPD_STATE _state)
{
	//状態変更
	updState_ = _state;

	//状態別更新関数を設定
	auto it = stateMap_.find(updState_);
	if (it != stateMap_.end())
	{
		selectUpdateFunc_ = it->second;
	}
	else
	{
		selectUpdateFunc_ = nullptr;
	}
}

void ModeSelect::SelectUpdate(SelectScene& _parent)

{
	constexpr int OFFSET_INDEX = 2;	//選択メニューのオフセット

	KeyConfig& key = KeyConfig::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, KeyConfig::JOYPAD_NO::PAD1))
	{
		_parent.ProcessMenuFunction(static_cast<SelectScene::SELECT_MENU>(menuIndex_));
		return;
	}

	//上へ
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		//新しく出るメニュー項目を変える
		SetMenuItem(menuIndex_ - OFFSET_INDEX, arcIndex_ - OFFSET_INDEX);

		//選択メニューの更新
		menuIndex_ = (menuIndex_ - 1 + SelectScene::MENU_LIST_NUM) % SelectScene::MENU_LIST_NUM;

		//円弧インデックスの更新
		arcIndex_ = (arcIndex_ - 1 + DRAW_ARC_NUM) % DRAW_ARC_NUM;

		//ターゲット角度の更新
		targetAngle_ += ROTATE_STEP;

		//状態変更
		ChangeUpdateState(UPD_STATE::ROTATE);

		return;
	}

	//下へ
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		//新しく出るメニュー項目を変える
		SetMenuItem(menuIndex_ + OFFSET_INDEX, arcIndex_ + OFFSET_INDEX);

		//選択メニューの更新		
		menuIndex_ = (menuIndex_ + 1) % SelectScene::MENU_LIST_NUM;

		//円弧インデックスの更新
		arcIndex_ = (arcIndex_ + 1) % DRAW_ARC_NUM;

		//ターゲット角度の更新
		targetAngle_ -= ROTATE_STEP;

		//状態変更
		ChangeUpdateState(UPD_STATE::ROTATE);

		return;
	}
}

void ModeSelect::RotateUpdate(SelectScene&)
{
	//回転
	float diff = targetAngle_ - currentAngle_;
	if (std::fabs(diff) <= ROTATE_SPEED)
	{
		currentAngle_ = targetAngle_;
		ChangeUpdateState(UPD_STATE::SELECT);
	}
	else
	{
		currentAngle_ += (diff > 0 ? ROTATE_SPEED : -ROTATE_SPEED);
	}

	//画像分割り当て
	for (int i = 0; i < DRAW_ARC_NUM; i++)
	{
		//オフセット角：等間隔
		arc_[i].angle = currentAngle_ + ROTATE_STEP * i;

		// 位置計算
		arc_[i].pos.x = ROTATE_CENTER_X + std::cos(arc_[i].angle) * ORBIT_RADIUS;
		arc_[i].pos.y = ROTATE_CENTER_Y + std::sin(arc_[i].angle) * ORBIT_RADIUS;
	}
}

void ModeSelect::SetMenuItem(const int _imgIndex, const int _arcIndex)
{
	//引数のインデックスが範囲外の場合の処理
	const int imgIndex = Utility::WrapIndex(_imgIndex, SelectScene::MENU_LIST_NUM);
	const int arcIndex = Utility::WrapIndex(_arcIndex, DRAW_ARC_NUM);

	//画像の割り当て
	arc_[arcIndex].img = imgArcs_[imgIndex];
}

void ModeSelect::DrawMessage()
{
	//描画位置
	constexpr float RATE = 0.5f;
	constexpr int POS_X = static_cast<int>(ResourceManager::SELECT_MES_SIZE_X * RATE / 2 + Application::SCREEN_HALF_X - 130);
	constexpr int POS_Y = 64;
	

	DrawRotaGraph(
		POS_X,
		POS_Y,
		RATE,
		0.0f,
		imgMessages_[static_cast<int>(SelectScene::SELECT_MES::MODE_SELECT)],
		true,
		false
	);
}

void ModeSelect::DrawDarkly(const int _index)
{
	constexpr int ALPHA = 128;
	
	// 画像描画
	DrawRotaGraph(
		arc_[_index].pos.x,
		arc_[_index].pos.y,
		1.0f,
		arc_[_index].angle,
		arc_[_index].img,
		true,
		false);

	// 描画前にブレンドモードを設定（乗算）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);

	// 画像描画
	DrawRotaGraph(
		arc_[_index].pos.x,
		arc_[_index].pos.y,
		1.0f,
		arc_[_index].angle,
		imgShadowArc_,
		true,
		false);

	//座標調整
	Vector2 pos = {
		arc_[_index].pos.x - imgArcSize.x / 2,
		arc_[_index].pos.y - imgArcSize.y / 2 };

	// ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ModeSelect::DrawGlow(const int _index)
{
	//座標調整
	Vector2 pos = { 
		arc_[_index].pos.x - imgArcSize.x / 2,
		arc_[_index].pos.y - imgArcSize.y / 2 };

	//描画設定
	material_->SetConstBuf(1,FLOAT4{ (float)imgArcSize.x, (float)imgArcSize.y,arc_[_index].angle, BLUR_DISTANCE });

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	renderer_->Draw(pos.x, pos.y);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	
	// 画像描画
	DrawRotaGraph(
		arc_[_index].pos.x,
		arc_[_index].pos.y,
		1.0f,
		arc_[_index].angle,
		arc_[_index].img,
		true,
		false);
}

void ModeSelect::DrawArrow()
{
	for (int i = 0; i < ARROWS; i++)
	{
		float angle = 0.0f;
		if (i == 1) { angle = Utility::Deg2RadF(180.0f); }

		DrawRotaGraph(
			ARROW_POS_X,
			ARROW_POS_Y[i],
			1.0f,
			angle,
			imgArrow_,
			true,
			false);
	}
}

void ModeSelect::DebugUpdate()
{
}

void ModeSelect::DebugDraw()
{
}
