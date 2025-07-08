#include"../Manager/Game/MapEditer.h"
#include"../Utility/Utility.h"
#include "../Common/ToonStyle.h"
#include "ItemBase.h"

ItemBase::ItemBase()
{
	status_.isBreak = false;
	status_.isGravity = false;
	status_.itemType = ITEM_TYPE::NONE;
	status_.effType = EFFECT_TYPE::FIXED;

	InitMapPos_ = INT_VECTOR_ZERO;

	rotY_ = 0.0f;

	movePow_ = Utility::VECTOR_ZERO;

	toonStyle_ = nullptr;
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

	//初期化
	InitShader();

	//モデルの更新
	trans_.Update();
}

void ItemBase::Draw(void)
{
	//カメラ範囲に含まれるか調べる
	if (IsInCameraView())
	{
		//含まれる場合
		return;	//描画を行わない
	}

	//モデル描画
	//MV1DrawModel(trans_.modelId);
	toonStyle_->Draw();
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

void ItemBase::ResetValue(void)
{
	//位置を初期位置に戻す
	trans_.pos = MapEditer::GetInstance().MapToWorldPos(InitMapPos_);
	//回転を0に
	//trans_.quaRot = Quaternion();

	//モデルへの反映
	trans_.Update();
}

void ItemBase::SetModelColor(const float _r, const float _g, const float _b, const float _a)
{
	//指定した色に変更
	toonStyle_->SetModelColor(_r, _g, _b, _a);

	//アウトラインのアルファ値も下げる
	toonStyle_->SetOutlineColor(1.0f, 1.0f, 1.0f, _a);
}

const VECTOR ItemBase::AdjustSizePer(const VECTOR _modelSize)const
{
	//マップエディタ
	MapEditer& map = MapEditer::GetInstance();

	//サイズ倍率
	VECTOR ret = Utility::VECTOR_ZERO;
	ret = map.MapToWorldPos(size_);

	//マップサイズ
	VECTOR modelSize = _modelSize;
	
	//サイズ倍率
	ret.x /= modelSize.x;
	ret.y /= modelSize.y;
	ret.z /= modelSize.z;

	return ret;
}

void ItemBase::InitShader()
{
	toonStyle_ = std::make_unique<ToonStyle>();
	toonStyle_->Load(trans_.modelId, ToonStyle::MESH_TYPE::MESH);
	toonStyle_->Init();
}

bool ItemBase::IsInCameraView()
{
	VECTOR boxPos1 = trans_.pos;
	VECTOR boxPos2 = MapEditer::GetInstance().MapToWorldPos(InitMapPos_ + mapSize_);

	if (CheckCameraViewClip_Box(boxPos1, boxPos2))
	{
		return true;
	}

	return false;
}