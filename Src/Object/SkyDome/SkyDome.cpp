#include "../../Common/Quaternion.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../../Utility/Utility.h"
#include "SkyDome.h"

SkyDome::SkyDome()
{
	state_ = STATE::NONE;
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Load(void)
{
	//リソース読み込み
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
}

void SkyDome::Init(void)
{
	// モデル制御の基本情報
	transform_.scl = SCALES;
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		Utility::Deg2RadF(180.0f),
		0.0f);
	transform_.quaRotLocal = Quaternion();

	//座標位置をずらす
	transform_.pos = { 
		MapEditer::GRID_SIZE * MapEditer::MAP_SIZE.x / 2, 
		0.0f, 
		MapEditer::GRID_SIZE* MapEditer::MAP_SIZE.z / 2 };
	transform_.Update();

	//Zバッファを無効
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);
}

void SkyDome::Update(void)
{
	Quaternion yRot = Quaternion::AngleAxis(Utility::Deg2RadF(ROT_SPEED), Utility::AXIS_Y);
	transform_.quaRot = Quaternion::Mult(transform_.quaRot, yRot);
	transform_.Update();
}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

const Transform& SkyDome::GetTransform() const
{
	return transform_;
}
