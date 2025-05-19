#include <string>
#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/InputManager.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&SelectScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&SelectScene::LoadingDraw, this);

	//状態別更新関数のセット
	RegisterArcState(UPD_STATE::SELECT, [this]() { SelectUpdate(); });
	RegisterArcState(UPD_STATE::ROTATE, [this]() { 	RotateUpdate(); });

	//選択したメニューごとの処理をセット
	menuFuncTable_ = {
		{SELECT_MENU::SOLO,[this]() 
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		}},
		{SELECT_MENU::MULTI,[this]() 
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		}},
		{SELECT_MENU::FREE,[this]() 
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		}},
		{SELECT_MENU::SETTINGS,[this]() 
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
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
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Load(void)
{
	//フォントの登録
	resMng_.Load(ResourceManager::SRC::DOT_FONT);
	buttnFontHandle_ = CreateFontToHandle("ベストテンDOT", FONT_SIZE, 0);

	//リソースの読み込み
	imgArcs_ = resMng_.Load(ResourceManager::SRC::ARCS).handleIds_;
}

void SelectScene::Init(void)
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
	arc_[DRAW_ARC_NUM -1].img = imgArcs_[MENU_LIST_NUM - 1];

	//選択メニューの初期化
	ChangeUpdateState(UPD_STATE::SELECT);

	//ターゲット角度を初期化
	targetAngle_ = 0.0f;
}

void SelectScene::RegisterArcState(const UPD_STATE _state, std::function<void()> _update)
{
	stateMap_[_state] = _update;
}

void SelectScene::ChangeUpdateState(const UPD_STATE _state)
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

void SelectScene::ArcRotation(const int _index)
{

}

void SelectScene::NormalUpdate(void)
{
#ifdef _DEBUG
	DebugUpdate();
#endif

	// 毎フレームのUpdate処理
	if (selectUpdateFunc_) selectUpdateFunc_();
}

void SelectScene::NormalDraw(void)
{
#ifdef _DEBUG
	DebugDraw();
#endif
}

void SelectScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&SelectScene::NormalUpdate, this);
	drawFunc_ = std::bind(&SelectScene::NormalDraw, this);
}

void SelectScene::SelectUpdate()
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

void SelectScene::RotateUpdate()
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

void SelectScene::DebugUpdate()
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void SelectScene::DebugDraw()
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

	for (int i = 0; i < DRAW_ARC_NUM; i++)
	{
		DrawRotaGraph(
			arc_[i].pos.x,
			arc_[i].pos.y,
			1.0f,
			arc_[i].angle,
			arc_[i].img,
			true,
			false
		);

		DrawFormatString(
			400,
			20*i,
			Utility::WHITE,
			"%d = ,%.2f,%d",
			i,
			arc_[i].angle,
			arc_[i].img
		);
	}

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
		Application::SCREEN_HALF_Y +16,
		Utility::WHITE,
		"arcIndex = %d",
		arcIndex_
	);
}

void SelectScene::SetMenuItem(const int _imgIndex, const int _arcIndex)
{
	
	//引数のインデックスが範囲外の場合の処理
	const int imgIndex = Utility::WrapIndex(_imgIndex, MENU_LIST_NUM);
	const int arcIndex = Utility::WrapIndex(_arcIndex, DRAW_ARC_NUM);

	//画像の割り当て
	arc_[arcIndex].img = imgArcs_[imgIndex];
}
