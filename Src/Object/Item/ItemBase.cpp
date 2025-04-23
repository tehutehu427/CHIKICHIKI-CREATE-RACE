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

void ItemBase::Init(IntVector3 _mapPos, Quaternion _quaRot, ITEM_TYPE _itemType)
{
	transform_.pos = VGet(_mapPos.x * 100 , _mapPos.y * 100, _mapPos.z * 100);
	transform_.quaRot = _quaRot;
	status_.itemType = _itemType;
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
	MV1DrawModel(transform_.modelId);
}
