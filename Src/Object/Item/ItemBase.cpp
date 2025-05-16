#include"../Manager/Game/MapEditer.h"
#include "ItemBase.h"

ItemBase::ItemBase()
{
	status_.isBreak = false;
	status_.isGravity = false;
	status_.itemType = ITEM_TYPE::NONE;
	status_.effType = EFFECT_TYPE::FIXED;
}

ItemBase::~ItemBase()
{
}

void ItemBase::Load()
{
}

void ItemBase::Init(IntVector3 _mapPos, Quaternion _quaRot, ITEM_TYPE _itemType)
{
	//マップ座標をワールド座標に変換して配置
	transform_.pos = MapEditer::GetInstance().MapToWorldPos(_mapPos);
	
	//回転
	transform_.quaRot = _quaRot;
	
	//アイテムの種類
	status_.itemType = _itemType;

	//基本初期化
	Init();
}

void ItemBase::Init()
{
	//個々の設定
	SetParam();

	//モデルの更新
	transform_.Update();
}

void ItemBase::Draw(void)
{
	//モデル描画
	MV1DrawModel(transform_.modelId);
}

void ItemBase::SetPos(IntVector3 mapPos)
{
	transform_.pos = MapEditer::GetInstance().MapToWorldPos(mapPos);
	transform_.Update();
}
