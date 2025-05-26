#include"../Manager/Game/MapEditer.h"
#include "ItemBase.h"

ItemBase::ItemBase()
{
	status_.isBreak = false;
	status_.isGravity = false;
	status_.itemType = ITEM_TYPE::NONE;
	status_.effType = EFFECT_TYPE::FIXED;

	InitMapPos_ = INT_VECTOR_ZERO;

	trans_.MakeCollider(Collider::TYPE::STAGE);
}

ItemBase::~ItemBase()
{
}

void ItemBase::Load()
{
}

void ItemBase::Init(IntVector3 _mapPos, Quaternion _quaRot, ITEM_TYPE _itemType)
{

	MapEditer& map = MapEditer::GetInstance();

	//マップ座標をワールド座標に変換して配置
	trans_.pos = map.MapToWorldPos(_mapPos);
	
	//グリッドと合わせるためにモデルサイズの半径分ずらす
	//trans_.localPos = ;
	
	//回転
	trans_.quaRot = _quaRot;
	
	//アイテムの種類
	status_.itemType = _itemType;

	//初期位置保存
	InitMapPos_ = _mapPos;

	//基本初期化
	Init();
}

void ItemBase::Init()
{
	//個々の設定
	SetParam();

	//モデルの更新
	trans_.Update();
}

void ItemBase::Draw(void)
{
	//モデル描画
	MV1DrawModel(trans_.modelId);
}

void ItemBase::Hit(Transform& _hitTrans)
{
}

void ItemBase::SetPos(IntVector3 mapPos)
{
	trans_.pos = MapEditer::GetInstance().MapToWorldPos(mapPos);
	trans_.Update();
}

void ItemBase::SetRotate(Quaternion rot)
{
	trans_.quaRot = rot;
	trans_.Update();
}
