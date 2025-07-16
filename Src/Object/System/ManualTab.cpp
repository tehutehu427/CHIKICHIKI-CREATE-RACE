#include "ManualTab.h"
#include <DxLib.h>
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Utility/Utility.h"



ManualTab::ManualTab(const Vector2& _padCursolPos) :
	key_(KeyConfig::GetInstance()),
	padCursorPos_(_padCursolPos)
{
	imgIcon_ = -1;
	index_ = 0;
	imgTriangle_ = -1;
	imgs_.clear();
	state_ = STATE::WAIT;
	uiPos_ = {};

	//処理の登録
	RegisterStateFunction(STATE::WAIT, SceneBase::ProcessFunction{ [&]() { UpdateWait(); },  [&]() { DrawWait(); } });
	RegisterStateFunction(STATE::DISPLAY, SceneBase::ProcessFunction{ [&]() { UpdateDisplay(); },  [&]() { DrawDisplay(); } });

	RegisterUpdateBySceneFunction(SceneManager::SCENE_ID::FREE, [this]() { UpdateFreeMode(); });
	RegisterUpdateBySceneFunction(SceneManager::SCENE_ID::SELECT, [this]() { UpdateSelectScene(); });

	RegisterLoadBySceneFunction(SceneManager::SCENE_ID::FREE, [this]() { LoadFreeResource(); });
	RegisterLoadBySceneFunction(SceneManager::SCENE_ID::SELECT, [this]() { LoadSelectResource(); });
}

ManualTab::~ManualTab()
{
}

void ManualTab::Load()
{
	//シーンの設定
	sceneId_ = SceneManager::GetInstance().GetSceneID();

	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgIcon_ = res.Load(ResourceManager::SRC::MANUAL_ICON).handleId_;
	loadFunc_[sceneId_]();
}

void ManualTab::Init()
{
	uiPos_ = {
		Application::SCREEN_SIZE_X - ICON_SIZE / 2,
		ICON_SIZE / 2,
	};
}

void ManualTab::Update()
{
	stateFunc_[state_].updataFunc_();
}

void ManualTab::Draw()
{
	stateFunc_[state_].drawFunc_();
}

void ManualTab::RegisterStateFunction(const STATE _state, SceneBase::ProcessFunction _func)
{
	stateFunc_[_state] = _func;
}

void ManualTab::RegisterUpdateBySceneFunction(const SceneManager::SCENE_ID _scene, std::function<void()> _func)
{
	updateFunc_[_scene] = _func;
}

void ManualTab::RegisterLoadBySceneFunction(const SceneManager::SCENE_ID _scene, std::function<void()> _func)
{
	loadFunc_[_scene] = _func;
}

void ManualTab::UpdateWait()
{
	
	if(key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL, KeyConfig::JOYPAD_NO::PAD1) ||
		key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL_ICON_CLICK, KeyConfig::JOYPAD_NO::PAD1) &&
		(Utility::IsPointInRectCircle(key_.GetMousePos(), uiPos_, RADIUS) || 
			Utility::IsPointInRectCircle(padCursorPos_, uiPos_, RADIUS)))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK);
		ChangeState(STATE::DISPLAY);
	}
}

void ManualTab::UpdateDisplay()
{
	updateFunc_[sceneId_]();
}

void ManualTab::DrawWait()
{
	//アイコンを描画
	DrawRotaGraph(
		uiPos_.x,
		uiPos_.y,
		1.0f,
		0.0f,
		imgIcon_,
		true,
		false
	);
}

void ManualTab::DrawDisplay()
{
	constexpr int OFFSET_POS_Y = 50;
	constexpr int ALPHA = 128;

	//背景を黒くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, Utility::BLACK, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	//マニュアルを描画
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y + OFFSET_POS_Y,
		1.0f,
		0.0f,
		imgs_[index_],
		true,
		false
	);

	if (imgTriangle_ != -1)
	{
		for (int i = 0; i < TRIANGLE_NUM; i++)
		{
			DrawRotaGraph(
				trianglePos_[i].x,
				trianglePos_[i].y,
				1.0f,
				Utility::Deg2RadF(i == 0 ? 270.0f : 90.0f),
				imgTriangle_,
				true,
				false
			);
		}
		
	}
}

void ManualTab::LoadSelectResource()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgs_.push_back(res.Load(ResourceManager::SRC::SELECT_MANUAL_KEY).handleId_);
	imgs_.push_back(res.Load(ResourceManager::SRC::SELECT_MANUAL_PAD).handleId_);

	index_ = GetJoypadNum() == 0 ? 0 : 1;
}

void ManualTab::LoadFreeResource()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgs_.push_back(res.Load(ResourceManager::SRC::FREE_MANUAL).handleId_);
	imgs_.push_back(res.Load(ResourceManager::SRC::FREE_MANUAL_2).handleId_);
	imgs_.push_back(res.Load(ResourceManager::SRC::FREE_MANUAL_3).handleId_);	
	imgTriangle_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;	
	
	constexpr int OFFSET_X = 350;
	for (int i = 0; i < TRIANGLE_NUM; i++)
	{
		int rev = i == 0 ? -1 : 1;
		trianglePos_[i].y = Application::SCREEN_HALF_Y;
		trianglePos_[i].x = Application::SCREEN_HALF_X + rev * OFFSET_X;
	}
}

void ManualTab::UpdateSelectScene()
{
	//インデックスの設定
	index_ = GetJoypadNum() == 0 ? 0 : 1;

	//キーの受付
	if (key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL, KeyConfig::JOYPAD_NO::PAD1) ||
		key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL_ICON_CLICK, KeyConfig::JOYPAD_NO::PAD1))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::CANCEL, SoundManager::PLAYTYPE::BACK);
		ChangeState(STATE::WAIT);
	}
}

void ManualTab::UpdateFreeMode()
{	

	if (key_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		index_ = (index_ - 1 + imgs_.size()) % imgs_.size();
		return;
	}
	else if (key_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		index_ = (index_ + 1) % imgs_.size();
		return;
	}

	//キーの受付
	if (key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL, KeyConfig::JOYPAD_NO::PAD1) ||
		key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL_ICON_CLICK, KeyConfig::JOYPAD_NO::PAD1))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::CANCEL, SoundManager::PLAYTYPE::BACK);
		ChangeState(STATE::WAIT);
	}
}

bool ManualTab::IsClickTriangle(const int _index)
{
	Vector2 leftTop = { trianglePos_[_index].x - TRIANGLE_SIZE / 2,trianglePos_[_index].y - TRIANGLE_SIZE / 2, };
	Vector2 rightBottom = { trianglePos_[_index].x + TRIANGLE_SIZE / 2,trianglePos_[_index].y + TRIANGLE_SIZE / 2, };

	if (Utility::IsPointInRect(padCursorPos_, leftTop, rightBottom))
	{
		return true;
	}

	return false;
}
