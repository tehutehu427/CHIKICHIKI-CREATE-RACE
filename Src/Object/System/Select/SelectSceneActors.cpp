#include "SelectSceneActors.h"
#include <DxLib.h>
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/Game/MapEditer.h"
#include "../../../Utility/Utility.h"
#include "../../Common/AnimationController.h"
#include "../../Common/ToonStyle.h"
#include "../../Player/Player.h"
#include "../../Item/Fixed/StartFlag.h"

SelectSceneActors::SelectSceneActors(void)
{
	chickenAnimationController_ = nullptr;
}

SelectSceneActors::~SelectSceneActors(void)
{
}

void SelectSceneActors::Load(void)
{
	ResourceManager & res = ResourceManager::GetInstance();

	//チキン----------------------------------------------	
	ACTOR type = ACTOR::CHICKEN;
	actorInfos_[type].transform.SetModel(
		res.LoadModelDuplicate(ResourceManager::SRC::CHICKEN));
	actorInfos_[type].transform.pos = CHICKEN_POS;
	actorInfos_[type].transform.quaRot = Quaternion();
	actorInfos_[type].transform.scl = Utility::VECTOR_ONE;
	actorInfos_[type].transform.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(0.0f), 0.0f });
	actorInfos_[type].transform.localPos = Utility::VECTOR_ZERO;
	actorInfos_[type].transform.Update();

	//トゥーン設定
	actorInfos_[type].toonStyle = std::make_unique<ToonStyle>();
	actorInfos_[type].toonStyle->Load(actorInfos_[type].transform.modelId, ToonStyle::MESH_TYPE::CHICKEN);	
	
	//アニメーションコントローラー
	chickenAnimationController_ = std::make_unique<AnimationController>(actorInfos_[ACTOR::CHICKEN].transform.modelId);

	//スタート----------------------------------------------
	type = ACTOR::START;
	actorInfos_[type].transform.SetModel(
		res.LoadModelDuplicate(ResourceManager::SRC::START_CUBE));
	actorInfos_[type].transform.pos = START_POS;
	actorInfos_[type].transform.quaRot = Quaternion();
	actorInfos_[type].transform.scl = Utility::VECTOR_ONE;
	actorInfos_[type].transform.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });
	actorInfos_[type].transform.localPos = Utility::VECTOR_ZERO;

	//マップエディタ
	MapEditer& map = MapEditer::GetInstance();

	//サイズ倍率
	VECTOR ret = Utility::VECTOR_ZERO;
	ret = map.MapToWorldPos(StartFlag::MAP_SIZE);

	//マップサイズ
	VECTOR modelSize = StartFlag::MODEL_SIZE;

	//サイズ倍率
	ret.x /= modelSize.x;
	ret.y /= modelSize.y;
	ret.z /= modelSize.z;

	//サイズ
	actorInfos_[type].transform.scl.x *= ret.x;
	actorInfos_[type].transform.scl.y *= ret.y;
	actorInfos_[type].transform.scl.z *= ret.z;
	
	actorInfos_[type].transform.Update();

	//トゥーン設定
	actorInfos_[type].toonStyle = std::make_unique<ToonStyle>();
	actorInfos_[type].toonStyle->Load(actorInfos_[type].transform.modelId, ToonStyle::MESH_TYPE::MESH);

}

void SelectSceneActors::Init(void)
{
	//チキンのアニメーション設定
	chickenAnimationController_->Add(CHICKEN_ANIM_INDEX_WAVE_HAND, Player::DEFAULT_ANIM_SPD);

	//アニメーション再生
	chickenAnimationController_->Play(CHICKEN_ANIM_INDEX_WAVE_HAND);

	//トゥーン初期化
	actorInfos_[ACTOR::CHICKEN].toonStyle->Init();
	actorInfos_[ACTOR::START].toonStyle->Init();
}

void SelectSceneActors::Update(void)
{
	//回転処理
	//チキンの位置をスタートの回転に合わせる
	ACTOR type = ACTOR::CHICKEN;

	//回転後の位置を計算
	VECTOR startPosF = VAdd(actorInfos_[ACTOR::START].transform.pos, VScale(actorInfos_[ACTOR::START].transform.GetBack(), DISTANCE));

	//Y以外の値を設定
	actorInfos_[type].transform.pos.x = startPosF.x;
	actorInfos_[type].transform.pos.z = startPosF.z;

	//各アクターの更新
	for (auto& [type, info] : actorInfos_)
	{
		//今回回転させたい回転量をクォータニオンで作る
		Quaternion rotPow = Quaternion();

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				Utility::Deg2RadF(ROT_DEG), Utility::AXIS_Y
			));

		// 回転諒を加える(合成)
		info.transform.quaRot = info.transform.quaRot.Mult(rotPow);

		info.transform.Update();
	}

	//アニメーション更新
	chickenAnimationController_->Update();
}

void SelectSceneActors::Draw(void)
{
	//各アクターの描画
	for (auto& [type, info] : actorInfos_)
	{
		info.toonStyle->Draw();
	}
}