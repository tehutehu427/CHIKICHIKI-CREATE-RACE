#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/Resource.h"
#include "../Manager/System/ResourceManager.h"
#include "CannonShot.h"

CannonShot::CannonShot(VECTOR _pos, Quaternion _quaRot)
{
	size_ = INT_VECTOR_ZERO;
	trans_.pos = _pos;
	trans_.quaRot = _quaRot;
	isAlive_ = false;
	movePow_ = Utility::VECTOR_ZERO;
	cnt_ = 0.0f;
}

CannonShot::~CannonShot()
{
}

void CannonShot::SetParam(void)
{
	//弾モデル
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CANNON_SHOT));

	//大きさ
	trans_.scl = { SCALE,SCALE,SCALE };

	//移動量
	movePow_ = VScale(trans_.quaRot.GetForward(), SPEED);

	//生存判定
	isAlive_ = true;
}

void CannonShot::Update(void)
{
	//弾の生存判定
	if (!isAlive_)return;

	//カウンタアップ
	cnt_ += SceneManager::GetInstance().GetDeltaTime();
	if (cnt_ >= ALIVE_TIME)
	{
		//消去処理
		Hit();

		return;
	}

	//移動
	Move();

	//モデル情報更新
	trans_.Update();
}

void CannonShot::Draw(void)
{
	//生きていないなら処理しない
	if (!isAlive_)return;

	//モデル描画
	MV1DrawModel(trans_.modelId);
}

void CannonShot::Hit(void)
{
	//弾を削除
	isAlive_ = false;
}

void CannonShot::Move(void)
{
	trans_.pos = VAdd(trans_.pos, movePow_);
}
