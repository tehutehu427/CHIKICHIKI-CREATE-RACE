#include <string>
#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Common/FontRegistry.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/InputManager.h"
#include "../Manager/System/DateBank.h"
#include "../Shader/Effect/SelectUIGlow.h"
#include "../Shader/Effect/SelectUIDarkly.h"
#include "ModeSelect.h"

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
	RegisterArcState(UPD_STATE::SELECT, [this]() { SelectUpdate(); });
	RegisterArcState(UPD_STATE::ROTATE, [this]() { 	RotateUpdate(); });

	//選択したメニューごとの処理をセット
	menuFuncTable_ = {
		{SELECT_MENU::SOLO,[this]()
		{
			dateBank_.SetPlayerNum(1);
			scnMng_.ChangeScene(SceneManager::SCENE_ID::SOLO);
		}},
		{SELECT_MENU::MULTI,[this]()
		{
			dateBank_.SetPlayerNum(4);
			scnMng_.ChangeScene(SceneManager::SCENE_ID::MULTI);
		}},
		{SELECT_MENU::FREE,[this]()
		{
			dateBank_.SetPlayerNum(1);
			scnMng_.ChangeScene(SceneManager::SCENE_ID::FREE);
		}},
		{SELECT_MENU::SETTINGS,[this]()
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}},
		{SELECT_MENU::EXIT,[this]()
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}}
	};

	//文字の初期化
	selectType_[static_cast<int>(SELECT_MENU::SOLO)] = "一人用課題モード";
	selectType_[static_cast<int>(SELECT_MENU::MULTI)] = "マルチモード";
	selectType_[static_cast<int>(SELECT_MENU::FREE)] = "フリーモード";
	selectType_[static_cast<int>(SELECT_MENU::SETTINGS)] = "システム設定";
	selectType_[static_cast<int>(SELECT_MENU::EXIT)] = "戻る";

	//変数の初期化
	for (int i = 0; i < DRAW_ARC_NUM; i++) { arc_[i] = {}; }
	int i = -1;
	imgArcs_ = &i;
	currentAngle_ = 0.0f;
	targetAngle_ = 0.0f;
	arcIndex_ = 0;
	menuIndex_ = 0;
	updState_ = UPD_STATE::NONE;
	uiGlow_ = nullptr;
	uiDarkly_ = nullptr;
	imgBackArc_ = 0;
	imgShadowArc_ = 0;
}

ModeSelect::~ModeSelect()
{
}

void ModeSelect::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();

	//リソースの読み込み
	imgArcs_ = res.Load(ResourceManager::SRC::ARCS).handleIds_;
	imgBackArc_ = res.Load(ResourceManager::SRC::BACK_ARC).handleId_;
	imgShadowArc_ = res.Load(ResourceManager::SRC::SHADOW_ARC).handleId_;

	uiGlow_ = std::make_unique<SelectUIGlow>();
	uiGlow_->Load();

	uiDarkly_ = std::make_unique<SelectUIDarkly>();
	uiDarkly_->Load();
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
	arc_[DRAW_ARC_NUM - 1].img = imgArcs_[MENU_LIST_NUM - 1];

	//選択メニューの初期化
	ChangeUpdateState(UPD_STATE::SELECT);

	//ターゲット角度を初期化
	targetAngle_ = 0.0f;
}

void ModeSelect::Update()
{
#ifdef _DEBUG
	DebugUpdate();
#endif

	// 毎フレームのUpdate処理
	if (selectUpdateFunc_) selectUpdateFunc_();

	//エフェクトを更新
	uiGlow_->Update(arc_[arcIndex_].angle, imgArcSize);
	uiDarkly_->Update(arcIndex_, arc_[arcIndex_].angle, imgArcSize, imgArcDivs);
}

void ModeSelect::Draw()
{
#ifdef _DEBUG
	DebugDraw();
#endif

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
}

void ModeSelect::RegisterArcState(const UPD_STATE _state, std::function<void()> _update)
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

void ModeSelect::SelectUpdate()
{
	constexpr int OFFSET_INDEX = 2;	//選択メニューのオフセット
	InputManager& ins = InputManager::GetInstance();
	//決定
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		//選択したメニューの処理を実行
		auto it = menuFuncTable_.find(static_cast<SELECT_MENU>(menuIndex_));
		if (it != menuFuncTable_.end())
		{
			it->second();
		}
	}
	//上へ
	else if (ins.IsTrgDown(KEY_INPUT_UP))
	{
		//新しく出るメニュー項目を変える
		SetMenuItem(menuIndex_ - OFFSET_INDEX, arcIndex_ - OFFSET_INDEX);

		//選択メニューの更新
		menuIndex_ = (menuIndex_ - 1 + MENU_LIST_NUM) % MENU_LIST_NUM;

		//円弧インデックスの更新
		arcIndex_ = (arcIndex_ - 1 + DRAW_ARC_NUM) % DRAW_ARC_NUM;

		//ターゲット角度の更新
		targetAngle_ += ROTATE_STEP;

		//状態変更
		ChangeUpdateState(UPD_STATE::ROTATE);
	}
	//下へ
	else if (ins.IsTrgDown(KEY_INPUT_DOWN))
	{
		//新しく出るメニュー項目を変える
		SetMenuItem(menuIndex_ + OFFSET_INDEX, arcIndex_ + OFFSET_INDEX);

		//選択メニューの更新		
		menuIndex_ = (menuIndex_ + 1) % MENU_LIST_NUM;

		//円弧インデックスの更新
		arcIndex_ = (arcIndex_ + 1) % DRAW_ARC_NUM;

		//ターゲット角度の更新
		targetAngle_ -= ROTATE_STEP;

		//状態変更
		ChangeUpdateState(UPD_STATE::ROTATE);
	}
}

void ModeSelect::RotateUpdate()
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
	const int imgIndex = Utility::WrapIndex(_imgIndex, MENU_LIST_NUM);
	const int arcIndex = Utility::WrapIndex(_arcIndex, DRAW_ARC_NUM);

	//画像の割り当て
	arc_[arcIndex].img = imgArcs_[imgIndex];
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

	//描画
	//uiGlow_->Draw(imgArcs_[_index], pos, imgArcSize);

	// ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ModeSelect::DrawGlow(const int _index)
{
	//座標調整
	Vector2 pos = { 
		arc_[_index].pos.x - imgArcSize.x / 2,
		arc_[_index].pos.y - imgArcSize.y / 2 };

	//描画
	uiGlow_->Draw(imgBackArc_, pos, imgArcSize);
	
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

void ModeSelect::DebugUpdate()
{
}

void ModeSelect::DebugDraw()
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		Utility::CYAN,
		true
	);

	DrawFormatString(
		0,
		0,
		Utility::BLACK,
		"SelectScene"
	);

	DrawFormatString(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		Utility::WHITE,
		"%d,%s",
		menuIndex_,
		selectType_[menuIndex_].c_str()
	);
	DrawFormatString(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y + 16,
		Utility::WHITE,
		"arcIndex = %d",
		arcIndex_
	);
}
