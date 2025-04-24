#include "../../Utility/Utility.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "EditController.h"

EditController::EditController()
{
	//変数初期化
	mousePos_ = Vector2();
	mapPos_ = Utility::VECTOR_ZERO;

	//モード管理(遷移時の初期処理)
	modeChanges_.emplace(MODE::ITEM_SELECT, std::bind(&EditController::ChengeModeItemSelect, this));
	modeChanges_.emplace(MODE::MOVE, std::bind(&EditController::ChengeModeMove, this));
	modeChanges_.emplace(MODE::ROTATE, std::bind(&EditController::ChengeModeRotate, this));
}

void EditController::Init(void)
{
	//モード変更
	ChengeMode(MODE::ITEM_SELECT);
}

void EditController::Update(void)
{
	//マウス位置取得
	mousePos_ = InputManager::GetInstance().GetMousePos();

	//モード別更新処理
	modeUpdate_();


	ItemNotSelect();
}

void EditController::Draw(void)
{
	modeDraw_();
}

void EditController::ChengeMode(MODE mode)
{
	if (mode_ != mode)
	{
		mode_ = mode;
		modeChanges_[mode_]();
	}
}

void EditController::ChengeModeItemSelect(void)
{
	modeUpdate_ = std::bind(&EditController::ItemSelectUpdate, this);
	modeDraw_ = std::bind(&EditController::ItemSelectDraw, this);
}

void EditController::ChengeModeMove(void)
{
	modeUpdate_ = std::bind(&EditController::MoveObjectUpdate, this);
	modeDraw_ = std::bind(&EditController::MoveObjectDraw, this);
}

void EditController::ChengeModeRotate(void)
{
	modeUpdate_ = std::bind(&EditController::RotateObjectUpdate, this);
	modeDraw_ = std::bind(&EditController::RotateObjectDraw, this);
}

void EditController::ItemSelectUpdate(void)
{
}

void EditController::MoveObjectUpdate(void)
{

}

void EditController::RotateObjectUpdate(void)
{
}

void EditController::ItemSelectDraw(void)
{
}

void EditController::MoveObjectDraw(void)
{
}

void EditController::RotateObjectDraw(void)
{
}

void EditController::ItemNotSelect(void)
{
	IntVector3 NearPos = NearObjectPos();
	if (isClickObject_ == true)
	{

	}
	else
	{
		//選択解除
		ChengeMode(MODE::ITEM_SELECT);
	}
}

IntVector3 EditController::NearObjectPos(void)
{
	isClickObject_ = false;
	IntVector3 mapPos = {-1,-1,-1};
	VECTOR mousePosNear3D = { mousePos_.x, mousePos_.y, 0.0f };
	VECTOR nearWorldPos = ConvScreenPosToWorldPos(mousePosNear3D);	//近いほうのワールド座標
	VECTOR mousePosFar3D = { mousePos_.x, mousePos_.y, 1.0f };
	VECTOR farWorldPos = ConvScreenPosToWorldPos(mousePosFar3D);	//遠いほうのワールド座標
	VECTOR normalmousePos3D = VNorm(VSub(farWorldPos, nearWorldPos));
	//遠いほうをマップの中に入れる
	while (farWorldPos.x < 0.0f || farWorldPos.x > MapEditer::MAP_SIZE.x * MapEditer::GRID_SIZE ||
		farWorldPos.y < 0.0f || farWorldPos.y > MapEditer::MAP_SIZE.y * MapEditer::GRID_SIZE||
		farWorldPos.z < 0.0f || farWorldPos.z > MapEditer::MAP_SIZE.z * MapEditer::GRID_SIZE)
	{
		farWorldPos = VSub(farWorldPos, normalmousePos3D);
	}
	for (float t = 0.0f; t < 1.0f; t += 0.01f)
	{
		VECTOR lerp = Utility::Lerp(nearWorldPos, farWorldPos, t);
		IntVector3 mapPosTemp = MapEditer::GetInstance().WorldToMapPos(lerp);
		if (MapEditer::GetInstance().GetItemType(mapPosTemp) != ItemBase::ITEM_TYPE::NONE)
		{
			mapPos = mapPosTemp;
			isClickObject_ = true;
			break;
		}
		else
		{
			mapPos = mapPosTemp;
		}
	}
	return mapPos;
}

