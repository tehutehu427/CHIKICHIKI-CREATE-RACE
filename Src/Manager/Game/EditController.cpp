#include "../../Utility/Utility.h"
#include "../System/InputManager.h"
#include "MapEditer.h"
#include "EditController.h"

EditController::EditController()
{
	mousePos_ = Vector2();
	mapPos_ = Utility::VECTOR_ZERO;
	//モード管理(遷移時の初期処理)
	modeChanges_.emplace(MODE::ITEM_SELECT, std::bind(&EditController::ChengeModeItemSelect, this));
	modeChanges_.emplace(MODE::MOVE, std::bind(&EditController::ChengeModeMove, this));
	modeChanges_.emplace(MODE::ROTATE, std::bind(&EditController::ChengeModeRotate, this));
	ChengeMode(MODE::ITEM_SELECT);
}

void EditController::Init(void)
{
	ChengeMode(MODE::ITEM_SELECT);
}

void EditController::Update(void)
{
	mousePos_ = InputManager::GetInstance().GetMousePos();
	modeUpdate_();
	ItemNotSelect();
}

void EditController::Draw(void)
{
	modeDraw_();
}

void EditController::Release(void)
{
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
	IntVector3 errorMapPos = {-1,-1,-1};
	//if (NearObjectPos() == errorMapPos  )
	if (NearObjectPos().x == errorMapPos.x
		&& NearObjectPos().y == errorMapPos.y
		&& NearObjectPos().z == errorMapPos.z)
	{
		//選択解除
		ChengeMode(MODE::ITEM_SELECT);
	}
}

IntVector3 EditController::NearObjectPos(void)
{
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
	return mapPos;
}

