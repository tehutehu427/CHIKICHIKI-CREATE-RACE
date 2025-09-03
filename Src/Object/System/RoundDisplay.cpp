#include "RoundDisplay.h"
#include <DxLib.h>
#include <cmath>
#include "../../Application.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/Game/ItemManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/Camera.h"
#include "../../Utility/Utility.h"

RoundDisplay::RoundDisplay()
{
	int i = -1;
	imgNumbers_ = &i;
	imgRound_ = -1;
	numberIndex_ = -1;
	numberDigit_ = -1;
	cameraPos_ = Utility::VECTOR_ZERO;
	cnt_ = 0.0f;
}

RoundDisplay::~RoundDisplay()
{
	
}

void RoundDisplay::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgRound_ = res.Load(ResourceManager::SRC::ROUND).handleId_;
	imgNumbers_ = res.Load(ResourceManager::SRC::NUMBERS).handleIds_;
}

void RoundDisplay::Init()
{
	//初期値
	numberIndex_ = 0;
	numberDigit_ = 0;
	cameraPos_ = CAMERA_OVERLOOKING_POS;
	cnt_ = 0.0f;
	//カメラ
	auto camera = SceneManager::GetInstance().GetCamera(0).lock();
	//カメラ固定
	camera->ChangeMode(Camera::MODE::FIXED_POINT);
}

void RoundDisplay::Update()
{
	//アルファ値を変更しつつ表示する
		
	//ステージの更新
	ItemManager::GetInstance().Update();

	//カメラの俯瞰位置を変える
	CameraOverLooking();
}

void RoundDisplay::Draw()
{
	//ステージの描画
	ItemManager::GetInstance().Draw();

	//縦座標を固定
	constexpr int ROUND_POS_X = Application::SCREEN_HALF_X - 100;
	constexpr int NUMBER_POS_X = Application::SCREEN_HALF_X + 230;
	constexpr int NUMBER_OFFSET_X = 70;
	constexpr int POS_Y = Application::SCREEN_HALF_Y;

	//ラウンドの描画
	DrawRotaGraph(
		ROUND_POS_X,
		POS_Y,
		1.0f,
		0.0f,
		imgRound_,
		true,
		false
	);

	//数字の描画
	for (int i = 0; i < numberDigit_; i++)
	{
		int index = Utility::GetDigit(numberIndex_, numberDigit_ - 1 - i);

		DrawRotaGraph(
			NUMBER_POS_X + NUMBER_OFFSET_X * i,
			POS_Y,
			1.0f,
			0.0f,
			imgNumbers_[index],
			true,
			false
		);
	}
}

void RoundDisplay::AddNumberIndex(const int _addIndex)
{
	//ナンバーインデックスを追加
	numberIndex_ += _addIndex;

	//桁数
	numberDigit_ = Utility::GetDigitCount(numberIndex_);
}

void RoundDisplay::CameraOverLooking(void)
{
	//カウンタ
	cnt_ += SceneManager::GetInstance().GetDeltaTime();

	//アイテムマネージャ
	auto& itemMng = ItemManager::GetInstance();
	
	//カメラ
	auto camera = SceneManager::GetInstance().GetCamera(0).lock();

	//スタート地点の座標
	VECTOR startPos = itemMng.GetStartObjectPos();
	
	//ゴール地点の座標
	VECTOR goalPos = itemMng.GetGoalObjectPos();
	
	//中心座標
	VECTOR centerPos = VScale(VAdd(startPos, goalPos), 0.5f);

	//カメラのターゲットを中心座標に
	camera->SetTargetPos(centerPos);

	Quaternion quaRot = Quaternion::AngleAxis(Utility::Deg2RadF(cnt_ * SPEED), Utility::AXIS_Y);
	VECTOR relative = VSub(CAMERA_OVERLOOKING_POS,centerPos);
	VECTOR rotated = quaRot.PosAxis(relative);

	//位置回転
	cameraPos_ = rotated;

	//カメラの位置設定
	camera->SetPos(cameraPos_);

	//カメラの俯瞰位置からベクトルを作る
	VECTOR angle = Utility::GetMoveVec(cameraPos_, centerPos);

	//カメラの角度決定
	camera->SetAngles(angle);
}
