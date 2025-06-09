#include "../../Utility/Utility.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "EditController.h"

EditController::EditController(int playerNum)
{
	playerNum_ = playerNum;
	mousePos_ = Vector2();
	mapPos_ = {};
	moveDir_ = MOVE_DIR::NONE;
	//モード管理(遷移時の初期処理)
	modeChanges_.emplace(MODE::ITEM_SELECT, std::bind(&EditController::ChengeModeItemSelect, this));
	modeChanges_.emplace(MODE::MOVE_ROTATE, std::bind(&EditController::ChengeModeMove, this));
	mapPosObject_ = ERROR_POS;
	mode_ = MODE::MAX;
	itemType_ = ItemBase::ITEM_TYPE::NONE;
	isClickObject_ = false;
	moveDir_ = MOVE_DIR::NONE;
}

EditController::~EditController()
{
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
	DebugUpdate();

	//モード別更新処理
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
	auto& itemMIns = ItemManager::GetInstance();
	if (MapEditer::GetInstance().IsObjectAtMapPos(mapPos_, itemMIns.GetDummyItemSize(playerNum_),itemMIns.GetDummyItemHitSize(playerNum_), itemMIns.GetDummyItemRotY(playerNum_)))
	{
		//アイテムが重なっている
		return;
	}
	//アイテムを追加
	//ダミーアイテムを変更する
	MapEditer::STATUS status;
	status.mapPos = mapPos_;
	status.rotate = itemMIns.GetDummyItemTransform(playerNum_).quaRot;
	status.type = itemType_;
	MapEditer::GetInstance().AddItem(status, itemMIns.GetDummyItemSize(playerNum_),itemMIns.GetDummyItemHitSize(playerNum_), itemMIns.GetDummyItemRotY(playerNum_));
	itemMIns.DummyItemAddItems(playerNum_);
	itemMIns.CreateDummyItem({}, {}, itemType, playerNum_);
	IntVector3 mapPos = NearObjectFrontPos();
	Quaternion rot = {};
	if (mapPos == ERROR_POS)
	{
		itemMIns.DeleteDummyItem(playerNum_);
		return;
	}
	itemMIns.CreateDummyItem(mapPos, rot, itemType_, playerNum_);
	mapPos_ = mapPos;
	ChengeMode(MODE::MOVE_ROTATE);
}

void EditController::ChengeModeItemSelect(void)
{
	modeUpdate_ = std::bind(&EditController::ItemSelectUpdate, this);
	modeDraw_ = std::bind(&EditController::ItemSelectDraw, this);
}

void EditController::ChengeModeMove(void)
{
	modeUpdate_ = std::bind(&EditController::MoveRotateObjectUpdate, this);
	modeDraw_ = std::bind(&EditController::MoveRotateObjectDraw, this);
}

void EditController::ItemSelectUpdate(void)
{
	//ItemNotSelect();
}

void EditController::MoveRotateObjectUpdate(void)
{
	RotateObject();
	moveDir_ = GetMoveDir();
	if (moveDir_ == MOVE_DIR::NONE)
	{
		return;
	}
	MoveItem();
}

void EditController::ItemSelectDraw(void)
{
}

void EditController::MoveRotateObjectDraw(void)
{
	if (mapPos_ != ERROR_POS)
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

void EditController::ItemNotSelect(void)
{
	auto& itemMIns = ItemManager::GetInstance();
	if (InputManager::GetInstance().IsTrgDownMouseLeft() == true)
	{
		if (MapEditer::GetInstance().IsObjectAtMapPos(mapPos_, itemMIns.GetDummyItemSize(playerNum_), itemMIns.GetDummyItemHitSize(playerNum_), itemMIns.GetDummyItemRotY(playerNum_)))
		{
			return;
		}
		if (itemMIns.IsDummyItem(playerNum_))
		{
			MapEditer::STATUS status;
			status.mapPos = mapPos_;
			status.rotate = itemMIns.GetDummyItemTransform(playerNum_).quaRot;
			status.type = itemType_;
			MapEditer::GetInstance().AddItem(status, itemMIns.GetDummyItemSize(playerNum_), itemMIns.GetDummyItemHitSize(playerNum_), itemMIns.GetDummyItemRotY(playerNum_));
			itemMIns.DummyItemAddItems(playerNum_);
		}
		IntVector3 NearPos = NearObjectFrontPos();
		if (isClickObject_ == true)
		{
			itemType_ = MapEditer::GetInstance().GetItemType(mapPosObject_);
			IntVector3 leaderPos = MapEditer::GetInstance().GetLeaderMapPos(mapPosObject_);
			//if (ItemManager::GetInstance().IsDummyItem(playerNum_))
			//{
			//	MapEditer::STATUS status;
			//	status.mapPos = mapPos_;
			//	status.rotate = ItemManager::GetInstance().GetDummyItemTransform(playerNum_).quaRot;
			//	status.type = itemType_;
			//	MapEditer::GetInstance().AddItem(status,ItemManager::GetInstance().GetDummyItemSize(playerNum_),ItemManager::GetInstance().GetDummyItemHitSize(playerNum_),ItemManager::GetInstance().GetDummyItemRotY(playerNum_));
			//}
			//ItemManager::GetInstance().DummyItemAddItems(playerNum_);
			if (!ItemManager::GetInstance().ItemsAddDummyItems(itemType_, leaderPos, playerNum_))
			{
				return;
			}
			MapEditer::GetInstance().DeleteItem(itemType_, leaderPos, ItemManager::GetInstance().GetDummyItemRotY(playerNum_), ItemManager::GetInstance().GetDummyItemSize(playerNum_),ItemManager::GetInstance().GetDummyItemHitSize(playerNum_));
			mapPos_ = leaderPos;
			ChengeMode(MODE::MOVE_ROTATE);
		}
		else
		{
			if (MapEditer::GetInstance().IsObjectAtMapPos(mapPos_, ItemManager::GetInstance().GetDummyItemSize(playerNum_),ItemManager::GetInstance().GetDummyItemHitSize(playerNum_),ItemManager::GetInstance().GetDummyItemRotY(playerNum_)))
			{
				return;
			}
			//アイテムを追加
			//MapEditer::STATUS status;
			//status.mapPos = mapPos_;
			//status.rotate = ItemManager::GetInstance().GetDummyItemTransform(playerNum_).quaRot;
			//status.type = itemType_;
			//MapEditer::GetInstance().AddItem(status,ItemManager::GetInstance().GetDummyItemSize(playerNum_),ItemManager::GetInstance().GetDummyItemHitSize(playerNum_),ItemManager::GetInstance().GetDummyItemRotY(playerNum_));
			itemType_ = ItemBase::ITEM_TYPE::NONE;
			//ItemManager::GetInstance().DummyItemAddItems(playerNum_);
			//選択解除
			ChengeMode(MODE::ITEM_SELECT);
		}
	}
}

IntVector3 EditController::NearObjectFrontPos(void)
{
	isClickObject_ = false;
	IntVector3 mapPos = ERROR_POS;
	VECTOR mousePosNear3D = { static_cast<float>(mousePos_.x),static_cast<float>(mousePos_.y), 0.0f };
	VECTOR nearWorldPos = ConvScreenPosToWorldPos(mousePosNear3D);	//近いほうのワールド座標
	VECTOR mousePosFar3D = { static_cast<float>(mousePos_.x),static_cast<float>(mousePos_.y), 1.0f };
	VECTOR farWorldPos = ConvScreenPosToWorldPos(mousePosFar3D);	//遠いほうのワールド座標
	VECTOR normalmousePos3D = VNorm(VSub(farWorldPos, nearWorldPos));
	//遠いほうをマップの中に入れる
	while (farWorldPos.x < 0.0f || farWorldPos.x > MapEditer::MAP_SIZE.x * MapEditer::GRID_SIZE ||
		farWorldPos.y < 0.0f || farWorldPos.y > MapEditer::MAP_SIZE.y * MapEditer::GRID_SIZE||
		farWorldPos.z < 0.0f || farWorldPos.z > MapEditer::MAP_SIZE.z * MapEditer::GRID_SIZE)
	{
		farWorldPos = VSub(farWorldPos, normalmousePos3D);
		VECTOR normalmousePos = VNorm(VSub(farWorldPos, nearWorldPos));
		if (Utility::GetSign(normalmousePos.x) != Utility::GetSign(normalmousePos3D.x) ||
			Utility::GetSign(normalmousePos.y) != Utility::GetSign(normalmousePos3D.y) ||
			Utility::GetSign(normalmousePos.z) != Utility::GetSign(normalmousePos3D.z))
		{
			return mapPos;
		}
	}
	auto size = ItemManager::GetInstance().GetDummyItemSize(playerNum_);
	//線形補間で当たり判定をする
	for (float t = 0.0f; t < 1.0f; t += 0.01f)
	{
		VECTOR lerp = Utility::Lerp(nearWorldPos, farWorldPos, t);
		IntVector3 mapPosTemp = MapEditer::GetInstance().WorldToMapPos(lerp);
		if (size == ERROR_POS)
		{
			if (MapEditer::GetInstance().IsObjectAtMapPos(mapPosTemp))
			{
				mapPosObject_ = mapPosTemp;
				isClickObject_ = true;
				return mapPos;
			}
		}
		else
		{
			//サイズ分の大きさを検査する
			for (int x = 0;x < size.x;x++)
			{
				for (int y = 0;y < size.y;y++)
				{
					for (int z = 0;z < size.z;z++)
					{
						IntVector3 sizeLoop = { x,y,z };
						if (MapEditer::GetInstance().IsObjectAtMapPos(mapPosTemp + sizeLoop))
						{
							if (mapPos.x < 0 || mapPos.x > MapEditer::MAP_SIZE.x - size.x ||
								mapPos.y < 0 || mapPos.y > MapEditer::MAP_SIZE.y - size.y ||
								mapPos.z < 0 || mapPos.z > MapEditer::MAP_SIZE.z - size.z)
							{
								return ERROR_POS;
							}
							isClickObject_ = true;
							mapPosObject_ = mapPosTemp + sizeLoop;
							return mapPos;
						}
					}
				}
			}
		}
		mapPos = mapPosTemp;
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
	VECTOR mousePosNear3D = {static_cast<float>(mousePos_.x),static_cast<float>(mousePos_.y), 0.0f };
	VECTOR nearWorldPos = ConvScreenPosToWorldPos(mousePosNear3D);	//近いほうのワールド座標
	VECTOR mousePosFar3D = {static_cast<float>(mousePos_.x),static_cast<float>(mousePos_.y), 1.0f };
	VECTOR farWorldPos = ConvScreenPosToWorldPos(mousePosFar3D);	//遠いほうのワールド座標
	VECTOR normalmousePos3D = VNorm(VSub(farWorldPos, nearWorldPos));
	VECTOR wallWorldPosNear = MapEditer::GetInstance().MapToWorldPos(nullWallMapPosNear);
	VECTOR wallWorldPosFar = MapEditer::GetInstance().MapToWorldPos(nullWallMapPosFar);
	//遠いほうを壁に当たる中に入れる
	farWorldPos = VSub(farWorldPos, normalmousePos3D);
	auto& itemIns = ItemManager::GetInstance();
	auto size = itemIns.GetDummyItemSize(playerNum_);
	switch (moveDir_)
	{
	case EditController::MOVE_DIR::NONE:
		return;
		break;
	case EditController::MOVE_DIR::X:
		while (farWorldPos.z < wallWorldPosNear.z || farWorldPos.z > wallWorldPosFar.z)
		{
			farWorldPos = VSub(farWorldPos, normalmousePos3D);
			VECTOR normalmousePos = VNorm(VSub(farWorldPos, nearWorldPos));
			if (Utility::GetSign(normalmousePos.x) != Utility::GetSign(normalmousePos3D.x) ||
				Utility::GetSign(normalmousePos.y) != Utility::GetSign(normalmousePos3D.y) ||
				Utility::GetSign(normalmousePos.z) != Utility::GetSign(normalmousePos3D.z))
			{
				return;
			}
		}
		mapPos_.x = MapEditer::GetInstance().WorldToMapPos(farWorldPos).x;
		mapPos_.x = mapPos_.x < 0 ? 0 : mapPos_.x >= MapEditer::MAP_SIZE.x - size.x ? MapEditer::MAP_SIZE.x - size.x : mapPos_.x;
		break;
	case EditController::MOVE_DIR::Y:
		while (farWorldPos.z < wallWorldPosNear.z || farWorldPos.z > wallWorldPosFar.z)
		{
			farWorldPos = VSub(farWorldPos, normalmousePos3D);
			VECTOR normalmousePos = VNorm(VSub(farWorldPos, nearWorldPos));
			if (Utility::GetSign(normalmousePos.x) != Utility::GetSign(normalmousePos3D.x) ||
				Utility::GetSign(normalmousePos.y) != Utility::GetSign(normalmousePos3D.y) ||
				Utility::GetSign(normalmousePos.z) != Utility::GetSign(normalmousePos3D.z))
			{
				return;
			}
		}
		mapPos_.y = MapEditer::GetInstance().WorldToMapPos(farWorldPos).y;
		mapPos_.y = mapPos_.y < 0 ? 0 : mapPos_.y >= MapEditer::MAP_SIZE.y - size.y ? MapEditer::MAP_SIZE.y - size.y: mapPos_.y;
		break;
	case EditController::MOVE_DIR::Z:
		while (farWorldPos.x < wallWorldPosNear.x || farWorldPos.x > wallWorldPosFar.x)
		{
			farWorldPos = VSub(farWorldPos, normalmousePos3D);
			VECTOR normalmousePos = VNorm(VSub(farWorldPos, nearWorldPos));
			if (Utility::GetSign(normalmousePos.x) != Utility::GetSign(normalmousePos3D.x) ||
				Utility::GetSign(normalmousePos.y) != Utility::GetSign(normalmousePos3D.y) ||
				Utility::GetSign(normalmousePos.z) != Utility::GetSign(normalmousePos3D.z))
			{
				return;
			}
		}
		mapPos_.z = MapEditer::GetInstance().WorldToMapPos(farWorldPos).z;
		mapPos_.z = mapPos_.z < 0 ? 0 : mapPos_.z >= MapEditer::MAP_SIZE.z - size.z? MapEditer::MAP_SIZE.z - size.z: mapPos_.z;
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
	itemIns.DummyItemSetMapPos(mapPos_, playerNum_);
	itemIns.ResetDummyItem(playerNum_,itemType_,mapPos_);

}

EditController::MOVE_DIR EditController::GetMoveDir(void) const
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
		SetItemType(ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR);
	}
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_RETURN))
	{
		ChengeMode(MODE::ITEM_SELECT);
	}
}

void EditController::DebugDraw(void)
{
	DrawFormatString(0, 0, 0x000000, "%d", static_cast<int>(mode_));
	DrawFormatString(0, 20, 0x000000, "%d", static_cast<int>(itemType_));
	DrawFormatString(0, 40, 0x000000, "%d,%d,%d",mapPos_.x,mapPos_.y,mapPos_.z);
	DrawFormatString(0, 60, 0x000000, "%d", static_cast<int>(GetMoveDir()));
	IntVector3 size = ItemManager::GetInstance().GetDummyItemSize(playerNum_);
	DrawFormatString(0, 80, 0x000000, "%d,%d,%d",size.x,size.y,size.z);
	DrawFormatString(0, 100, 0x000000, "%d", static_cast<int>(ItemManager::GetInstance().GetDummyItemRotY(playerNum_)));

}

void EditController::RotateObject(void) const
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_R))
	{
		Quaternion rot = ItemManager::GetInstance().GetDummyItemTransform(playerNum_).quaRot;
		float rotScale = Utility::Deg2RadF(90.0f);
		ItemManager::GetInstance().SetDummyItemRotY(playerNum_, ItemManager::GetInstance().GetDummyItemRotY(playerNum_) + 90.0f);
		rot = Quaternion::Mult(rot, Quaternion::AngleAxis(rotScale,Utility::AXIS_Y));
		auto& itemIns = ItemManager::GetInstance();
		ItemManager::GetInstance().DummyItemSetRotate(rot, playerNum_);
		ItemManager::GetInstance().ResetDummyItem(playerNum_, itemType_,mapPos_);
	}
}

