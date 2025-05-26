#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game./MapEditer.h"
#include "Fence.h"

Fence::Fence()
{
}

Fence::~Fence()
{
}

void Fence::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FENCE));

	//相対座標
	trans_.localPos = MAP_LOCALPOS;

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void Fence::Update(void)
{
}

void Fence::Draw(void)
{
	//マップエディタ
	auto& map = MapEditer::GetInstance();

	//四角描画
	DrawCube3D(VSub(trans_.pos, VScale(map.MapToWorldPos(size_), 0.5f)),
		VAdd(trans_.pos, VScale(map.MapToWorldPos(size_), 0.5f)), 
			0xffffff, 0xffffff, true);
}
