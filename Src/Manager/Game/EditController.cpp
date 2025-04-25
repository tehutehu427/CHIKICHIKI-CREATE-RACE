#include "../../Utility/Utility.h"
#include "../System/InputManager.h"
#include "MapEditer.h"
#include "EditController.h"

EditController::EditController(int playerNum)
{
	playerNum_ = playerNum;
	mousePos_ = Vector2();
	mapPos_ = {-1,-1,-1};
	itemType_ = ItemBase::ITEM_TYPE::NONE;
	//モード管理(遷移時の初期処理)
	modeChanges_.emplace(MODE::ITEM_SELECT, std::bind(&EditController::ChengeModeItemSelect, this));
	modeChanges_.emplace(MODE::MOVE, std::bind(&EditController::ChengeModeMove, this));
	modeChanges_.emplace(MODE::ROTATE, std::bind(&EditController::ChengeModeRotate, this));
	ChengeMode(MODE::ITEM_SELECT);
}

void EditController::Init(void)
{
	moveDir_ = MOVE_DIR::NONE;
	ChengeMode(MODE::ITEM_SELECT);
}

void EditController::Update(void)
{
	mousePos_ = InputManager::GetInstance().GetMousePos();
	DebugUpdate();
	modeUpdate_();
	if (moveDir_ == MOVE_DIR::NONE)
	{
		ItemNotSelect();
	}
}

void EditController::Draw(void)
{
	DebugDraw();
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

void EditController::SetItemType(ItemBase::ITEM_TYPE itemType)
{
	itemType_ = itemType;
	if (itemType_ == ItemBase::ITEM_TYPE::NONE)
	{
		return;
	}
	//アイテムを追加
	IntVector3 mapPos = NearObjectPos();
	Quaternion rot = {};
	ItemManager::GetInstance().CreateDummyItem(mapPos, rot, itemType_, playerNum_);
	mapPos_ = mapPos;
	ChengeMode(MODE::MOVE);
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
	ItemNotSelect();
}

void EditController::MoveObjectUpdate(void)
{
	moveDir_ = GetMoveDir();
	if (moveDir_ == MOVE_DIR::NONE)
	{
		return;
	}
	MoveItem();
}

void EditController::RotateObjectUpdate(void)
{
}

void EditController::ItemSelectDraw(void)
{
}

void EditController::MoveObjectDraw(void)
{
	if (mapPos_ != IntVector3{-1,-1,-1})
	{
		VECTOR worldPos = MapEditer::GetInstance().MapToWorldPos(mapPos_);
		worldPos = VAdd(worldPos,{MapEditer::GRID_SIZE /2 ,MapEditer::GRID_SIZE / 2 ,MapEditer::GRID_SIZE / 2 });
		DrawLine3D(worldPos, VAdd(worldPos, VScale(Utility::DIR_R,MOVE_ARROW_LENGTH)), 0x0000ff);	//X軸の線
		DrawSphere3D(VAdd(worldPos, VScale(Utility::DIR_R, MOVE_ARROW_LENGTH)), MOVE_ARROW_SIZE,32, 0x0000ff, 0x0000ff,true);	//X軸の先端
		DrawLine3D(worldPos, VAdd(worldPos, VScale(Utility::DIR_U, MOVE_ARROW_LENGTH)), 0x00ff00);	//Y軸の線
		DrawSphere3D(VAdd(worldPos, VScale(Utility::DIR_U, MOVE_ARROW_LENGTH)), MOVE_ARROW_SIZE, 32, 0x00ff00, 0x00ff00, true);	//Y軸の先端
		DrawLine3D(worldPos, VAdd(worldPos, VScale(Utility::DIR_F, MOVE_ARROW_LENGTH)), 0xff0000);	//Z軸の線
		DrawSphere3D(VAdd(worldPos, VScale(Utility::DIR_F, MOVE_ARROW_LENGTH)), MOVE_ARROW_SIZE, 32, 0xff0000, 0xff0000, true);	//Z軸の先端
	}
}

void EditController::RotateObjectDraw(void)
{
}

void EditController::ItemNotSelect(void)
{
	if (InputManager::GetInstance().IsTrgDownMouseLeft() == true)
	{
		IntVector3 NearPos = NearObjectPos();
		if (isClickObject_ == true)
		{
			itemType_ = MapEditer::GetInstance().GetItemType(NearPos);
			ChengeMode(MODE::MOVE);
		}
		else
		{
			itemType_ = ItemBase::ITEM_TYPE::NONE;
			ItemManager::GetInstance().DummyItemAddItems(playerNum_);
			//選択解除
			ChengeMode(MODE::ITEM_SELECT);
		}
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

void EditController::MoveItem(void)
{
	IntVector3 nullWallMapPosNear = { 0,0,0 };	//透明な壁を置く
	IntVector3 nullWallMapPosFar = MapEditer::MAP_SIZE;	//透明な壁を置く
	switch (moveDir_)
	{
	case EditController::MOVE_DIR::NONE:
		return;
		break;
	case EditController::MOVE_DIR::X:
	case EditController::MOVE_DIR::Y:
		nullWallMapPosNear.z = mapPos_.z;	//Z軸固定の壁を置く
		nullWallMapPosFar.z = mapPos_.z + 1;	//Z軸固定の壁を置く
		break;
	case EditController::MOVE_DIR::Z:
		nullWallMapPosNear.x = mapPos_.x;
		nullWallMapPosFar.x = mapPos_.x + 1;
		break;
	case EditController::MOVE_DIR::XY:
		break;
	case EditController::MOVE_DIR::XZ:
		break;
	case EditController::MOVE_DIR::YZ:
		break;
	default:
		break;
	}
	VECTOR mousePosNear3D = { mousePos_.x, mousePos_.y, 0.0f };
	VECTOR nearWorldPos = ConvScreenPosToWorldPos(mousePosNear3D);	//近いほうのワールド座標
	VECTOR mousePosFar3D = { mousePos_.x, mousePos_.y, 1.0f };
	VECTOR farWorldPos = ConvScreenPosToWorldPos(mousePosFar3D);	//遠いほうのワールド座標
	VECTOR normalmousePos3D = VNorm(VSub(farWorldPos, nearWorldPos));
	VECTOR wallWorldPosNear = MapEditer::GetInstance().MapToWorldPos(nullWallMapPosNear);
	VECTOR wallWorldPosFar = MapEditer::GetInstance().MapToWorldPos(nullWallMapPosFar);
	//遠いほうを壁に当たる中に入れる
	farWorldPos = VSub(farWorldPos, normalmousePos3D);
	switch (moveDir_)
	{
	case EditController::MOVE_DIR::NONE:
		return;
		break;
	case EditController::MOVE_DIR::X:
		while (farWorldPos.z < wallWorldPosNear.z || farWorldPos.z > wallWorldPosFar.z)
		{
			farWorldPos = VSub(farWorldPos, normalmousePos3D);
		}
		mapPos_.x = MapEditer::GetInstance().WorldToMapPos(farWorldPos).x;
		break;
	case EditController::MOVE_DIR::Y:
		while (farWorldPos.z < wallWorldPosNear.z || farWorldPos.z > wallWorldPosFar.z)
		{
			farWorldPos = VSub(farWorldPos, normalmousePos3D);
		}
		mapPos_.y = MapEditer::GetInstance().WorldToMapPos(farWorldPos).y;
		break;
	case EditController::MOVE_DIR::Z:
		while (farWorldPos.x < wallWorldPosNear.x || farWorldPos.x > wallWorldPosFar.x)
		{
			farWorldPos = VSub(farWorldPos, normalmousePos3D);
		}
		mapPos_.z = MapEditer::GetInstance().WorldToMapPos(farWorldPos).z;
		break;
	case EditController::MOVE_DIR::XY:
		break;
	case EditController::MOVE_DIR::XZ:
		break;
	case EditController::MOVE_DIR::YZ:
		break;
	default:
		break;
	}
	ItemManager::GetInstance().DummyItemSetMapPos(mapPos_, playerNum_);

}

EditController::MOVE_DIR EditController::GetMoveDir(void)
{
	MOVE_DIR moveDir = MOVE_DIR::NONE;
	InputManager& ins = InputManager::GetInstance();
	if (moveDir_ == MOVE_DIR::NONE)
	{
		if (ins.IsClickMouseLeft() == false)
		{
			return moveDir;
		}
		if (itemType_ == ItemBase::ITEM_TYPE::NONE)
		{
			return moveDir;
		}
	}
	else if (ins.IsTrgUpMouseLeft() == true)
	{
		return moveDir;
	}
	else
	{
		return moveDir_;
	}
	Vector2 mousePos = ins.GetMousePos();
	VECTOR worldPos = MapEditer::GetInstance().MapToWorldPos(mapPos_);
	worldPos = VAdd(worldPos, { MapEditer::GRID_SIZE / 2 ,MapEditer::GRID_SIZE / 2 ,MapEditer::GRID_SIZE / 2 });
	VECTOR x = ConvWorldPosToScreenPos(VAdd(worldPos, VScale(Utility::DIR_R, MOVE_ARROW_LENGTH)));
	VECTOR y = ConvWorldPosToScreenPos(VAdd(worldPos, VScale(Utility::DIR_U, MOVE_ARROW_LENGTH)));
	VECTOR z = ConvWorldPosToScreenPos(VAdd(worldPos, VScale(Utility::DIR_F, MOVE_ARROW_LENGTH)));
	//X軸移動の球をクリックした場合
	if (x.z > 0.0f || x.z < 1.0f)
	{
		Vector2 x2D = { static_cast<int>(x.x),static_cast<int>( x.y) };
		if (Utility::Distance(mousePos, x2D) < DELAY_MOVE_ARROW)
		{
			moveDir = MOVE_DIR::X;
			return moveDir;
		}
	}
	//Y軸移動の球をクリックした場合
	if (y.z > 0.0f || y.z < 1.0f)
	{
		Vector2 y2D = {static_cast<int>( y.x),static_cast<int>( y.y )};
		if (Utility::Distance(mousePos, y2D) < DELAY_MOVE_ARROW)
		{
			moveDir = MOVE_DIR::Y;
			return moveDir;
		}
	}
	//Z軸移動の球をクリックした場合
	if (z.z > 0.0f || z.z < 1.0f)
	{
		Vector2 z2D = {static_cast<int>( z.x),static_cast<int>( z.y )};
		if (Utility::Distance(mousePos, z2D) < DELAY_MOVE_ARROW)
		{
			moveDir = MOVE_DIR::Z;
			return moveDir;
		}
	}
	return moveDir;
}

void EditController::DebugUpdate(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_X))
	{
		SetItemType(ItemBase::ITEM_TYPE::FLOOR);
	}
}

void EditController::DebugDraw(void)
{
	DrawFormatString(0, 0, 0xffffff, "%d", static_cast<int>(mode_));
	DrawFormatString(0, 20, 0xffffff, "%d", static_cast<int>(itemType_));
	DrawFormatString(0, 40, 0xffffff, "%d,%d,%d",mapPos_.x,mapPos_.y,mapPos_.z);
	DrawFormatString(0, 60, 0xffffff, "%d", static_cast<int>(GetMoveDir()));

}

