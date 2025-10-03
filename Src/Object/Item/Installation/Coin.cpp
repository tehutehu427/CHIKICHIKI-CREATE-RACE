#include <algorithm>
#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SoundManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Manager/Game/MapEditer.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Common/EffectController.h"
#include "../../Player/Player.h"
#include "CoinFollower.h"
#include "Coin.h"

Coin::Coin(void)
{
	isEnd_ = false;
	followPos_ = Utility::VECTOR_ZERO;
}

Coin::~Coin(void)
{
}

void Coin::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::COIN));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;
	status_.rotType = ROTATION_TYPE::HALF_ROTATION;

	//サイズ
	trans_.scl = VScale(trans_.scl, SIZE_MULTI);

	//相対座標
	trans_.localPos.x = MapEditer::GetInstance().MapToWorldPos(MAP_SIZE).x / 2;
	trans_.localPos.y = MapEditer::GetInstance().MapToWorldPos(MAP_SIZE).y / 2;
	trans_.localPos.z = MapEditer::GetInstance().MapToWorldPos(MAP_SIZE).z / 2;

	//コライダの作成
	std::unique_ptr<Sphere> geo = std::make_unique<Sphere>(trans_.overAllPos, RADIUS);
	MakeCollider({ Collider::TAG::COIN }, std::move(geo), { Collider::TAG::PUNCH });

	//マップサイズ
	mapSize_ = MAP_SIZE;

	//終了判定
	isEnd_ = false;

	//エフェクト
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::COIN_GOAL_EFF).handleId_, EffectController::EFF_TYPE::COIN_GOAL);

	//SE
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::COIN_GET_SE);
}

void Coin::Update(void)
{
	//追従更新
	if (follower_ != nullptr)
	{
		follower_->Update();
	}

	//コインの役割が終わったら
	if (isEnd_)
	{
		//マップからも消す
		Delete();
		return;
	}

	//回転
	trans_.quaRot = trans_.quaRot.Mult(Quaternion::AngleAxis(Utility::Deg2RadF(ROTATE_SPEED), Utility::AXIS_Y));

	//モデル更新
	trans_.Update();

	//追従対象がいないなら
	if (followCol_.lock() == nullptr)
	{
		//コライダがないなら再生成
		if (colParam_.empty())
		{
			//コライダの作成
			std::unique_ptr<Sphere> geo = std::make_unique<Sphere>(trans_.overAllPos, RADIUS);
			MakeCollider({ Collider::TAG::COIN }, std::move(geo), { Collider::TAG::PUNCH });
		}

		//削除
		followCol_.reset();

		//何もしない
		return;
	}

	//追従座標
	VECTOR followPos = followCol_.lock()->GetParent().GetTransform().pos;
	Quaternion followRot = followCol_.lock()->GetParent().GetTransform().quaRot;
	VECTOR followLocalPos = followRot.PosAxis(followPos_);

	//対象に追従
	trans_.pos = VSub(VAdd(followPos, followLocalPos), trans_.localPos);
}

void Coin::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//すでに対象についているならスキップ
	if (followCol_.lock() != nullptr)return;

	//タグが影ならスキップ
	auto tags = _hitCol.lock()->GetTags();
	if (std::find(tags.begin(), tags.end(), Collider::TAG::SHADOW) != tags.end())return;

	//追従
	followCol_ = _hitCol;

	//フォロワー設定
	const Player& player = dynamic_cast<const Player&>(followCol_.lock()->GetParent());
	follower_ = std::make_unique<CoinFollower>(*this, player);
	followPos_ = VGet(FOLLOW_LOCAL_POS.x, FOLLOW_LOCAL_POS.y, FOLLOW_LOCAL_POS.z - COIN_DIS * player.GetCoinNum());

	//ゲットSE
	SoundManager::GetInstance().Play(SoundManager::SRC::COIN_GET_SE, SoundManager::PLAYTYPE::BACK);

	//コライダの消去
	colParam_[0].collider_->Kill();
	colParam_.clear();
}

void Coin::End(void)
{
	//終了
	isEnd_ = true;

	//エフェクト再生
	effect_->Play(EffectController::EFF_TYPE::COIN_GOAL, trans_.pos, trans_.quaRot, VGet(EFFECT_SCALE, EFFECT_SCALE, EFFECT_SCALE));
}

void Coin::Delete(void)
{
	//マップからの削除
	MapEditer::GetInstance().DeleteItem(initMapPos_, rotY_, size_, GetHitSize());

	//アイテムマネージャからの削除
	ItemManager::GetInstance().DeleteItem(initMapPos_,status_.itemType);
}
