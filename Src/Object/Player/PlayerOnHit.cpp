#include "../../Object/Common/Geometry/Sphere.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/Game/ScoreManager.h"
#include "../../Object/Common/Geometry/Line.h"
#include"../../Object/Common/Geometry/Model.h"
#include"../../Utility/Utility.h"
#include"./Player.h"
#include "PlayerOnHit.h"

PlayerOnHit::PlayerOnHit(PlayerAction& _action, std::vector<ObjectBase::ColParam>& _colParam, Transform& _trans,Collider::TAG _tag):
	action_(_action),
	colParam_(_colParam),
	trans_(_trans),
	tag_(_tag)
{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;
	colUpdates_.emplace(TAG::START, [this](const std::weak_ptr<Collider> _hitCol) {CollFloor(_hitCol); });
	colUpdates_.emplace(TAG::GOAL, [this](const std::weak_ptr<Collider> _hitCol) {ColGoal(_hitCol); });
	colUpdates_.emplace(TAG::NORMAL_ITEM, [this](const std::weak_ptr<Collider> _hitCol) {CollFloor(_hitCol); });
	colUpdates_.emplace(TAG::KILLER_SPECIFIC, [this](const std::weak_ptr<Collider> _hitCol) {CollKillerItemSpecific(_hitCol); });
	colUpdates_.emplace(TAG::KILLER_ALL, [this](const std::weak_ptr<Collider> _hitCol) {CollKillerItemAll(); });
	colUpdates_.emplace(TAG::MOVE_HORI_FLOOR, [this](const std::weak_ptr<Collider> _hitCol) {CollMoveFloor(_hitCol); });
	colUpdates_.emplace(TAG::MOVE_VER_FLOOR, [this](const std::weak_ptr<Collider> _hitCol) {CollMoveFloor(_hitCol); });
	colUpdates_.emplace(TAG::SLIME_FLOOR, [this](const std::weak_ptr<Collider> _hitCol) {CollSlimeFloor(_hitCol); });
	colUpdates_.emplace(TAG::WIND, [this](const std::weak_ptr<Collider> _hitCol) {CollWind(_hitCol); });
	colUpdates_.emplace(TAG::PUNCH, [this](const std::weak_ptr<Collider> _hitCol) {ColPunch(_hitCol); });
	colUpdates_.emplace(TAG::SPRING, [this](const std::weak_ptr<Collider> _hitCol) {ColSpring(_hitCol); });
	colUpdates_.emplace(TAG::CANNON_AIM, [this](const std::weak_ptr<Collider> _hitCol) {CollNone(); });
	colUpdates_.emplace(TAG::SHADOW, [this](const std::weak_ptr<Collider> _hitCol) {CollNone(); });
	colUpdates_.emplace(TAG::COIN, [this](const std::weak_ptr<Collider> _hitCol) {CollCoin(); });

	

	int playerNum = DateBank::GetInstance().GetPlayerNum();
	for (int i = static_cast<int>(TAG::PLAYER1); i < playerNum; i++)
	{
		//同じタグだったら設定しない
		if (static_cast<int>(tag_)== i)continue;
		colUpdates_[static_cast<TAG>(i)] = [this](const std::weak_ptr<Collider> _hitCol) {CollNone(); };
	}
	isGoal_ = false;
	isDeath_ = false;
	isLandHit_ = false;

	isHitSlimeFloor_ = false;
	moveDiff_ = Utility::VECTOR_ZERO;
	movedPos_ = Utility::VECTOR_ZERO;
	isSide_ = false;
	coinNum_ = 0;
}

PlayerOnHit::~PlayerOnHit(void)
{

}

void PlayerOnHit::Load(void)
{
	
}

void PlayerOnHit::Init(void)
{
	isGoal_ = false;
	isDeath_ = false;
	coinNum_ = 0;
}

void PlayerOnHit::Update(void)
{
	movedPos_ = VAdd(trans_.pos, action_.GetMovePow());
	movedPos_ = VAdd(movedPos_, action_.GetJumpPow());

	//移動量ラインの更新
	VECTOR moveVec = VSub(movedPos_, trans_.pos);
	moveVec.y -= MOVE_LINE_Y_OFFSET;
	if (moveVec.x != 0.0f || moveVec.y != MOVE_LINE_Y_CHECK_VALUE || moveVec.z != 0.0f)
	{
		Line& moveLine = dynamic_cast<Line&>(colParam_[MOVE_LINE_COL_NO].collider_->GetGeometry());
		moveLine.SetLocalPosPoint1(Utility::VECTOR_ZERO);
		moveLine.SetLocalPosPoint2(moveVec);
	}

	//移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;


}

void PlayerOnHit::ColUpdate(const std::weak_ptr<Collider> _hitCol)
{
	for (const auto tag : _hitCol.lock()->GetTags())
	{
		colUpdates_[tag](_hitCol);
	}
}

inline void PlayerOnHit::CollNone(void)
{
	//何もしない
}

void PlayerOnHit::CollFloor(const std::weak_ptr<Collider> _hitCol)
{
	HitModelCommon(_hitCol);
}

void PlayerOnHit::CollMoveFloor(const std::weak_ptr<Collider> _hitCol)
{
	ItemBase& floor = dynamic_cast<ItemBase&>(const_cast<ObjectBase&>(_hitCol.lock()->GetParent()));
	VECTOR movePow = floor.GetMovePow();
	movedPos_ = VAdd(movedPos_, floor.GetMovePow());
	HitModelCommon(_hitCol);
}

void PlayerOnHit::CollSlimeFloor(const std::weak_ptr<Collider> _hitCol)
{
	HitModelCommon(_hitCol);
	isHitSlimeFloor_ = true;
}

void PlayerOnHit::CollKillerItemSpecific(const std::weak_ptr<Collider> _hitCol)
{
	HitModelCommon(_hitCol);
	//地面に立っていたら
	if (isLandHit_)
	{
		isDeath_ = true;
	}
}

void PlayerOnHit::CollKillerItemAll(void)
{
	//当たったら死ぬ
	isDeath_ = true;
}

void PlayerOnHit::CollWind(const std::weak_ptr<Collider> _hitCol)
{
	//風オブジェクトから移動量を取得
	ItemBase& wind = dynamic_cast<ItemBase&>(const_cast<ObjectBase&>(_hitCol.lock()->GetParent()));
	VECTOR movePow = wind.GetMovePow();
	movedPos_ = VAdd(movedPos_, movePow);
	trans_.pos = movedPos_;
}

void PlayerOnHit::ColPunch(const std::weak_ptr<Collider> _hitCol)
{
	if (action_.GetAct() == PlayerAction::ATK_ACT::KNOCKBACK||colParam_[EYE_LINE_NO].collider_->IsHit())return;
	//リソースID
	auto& res = ResourceManager::GetInstance();
	//パンチしたプレイヤーの向いてる方向をセットする
 	VECTOR punchedPlayerPos = _hitCol.lock()->GetParent().GetTransform().pos;

	//パンチしたプレイヤーの位置と自分の位置を比較して、
	action_.SetDir(Utility::GetMoveVec(punchedPlayerPos, trans_.pos));

	//ノックバック状態遷移
	action_.ChangeAction(PlayerAction::ATK_ACT::KNOCKBACK);
}

void PlayerOnHit::ColSpring(const std::weak_ptr<Collider> _hitCol)
{
	//ジャンプ力の設定
	HitModelCommon(_hitCol);
	//リソースID
	auto& res = ResourceManager::GetInstance();
	if (!isSide_)
	{
		//バネジャンプ音再生
		action_.SetStepJump(0.0f);
		SoundManager::GetInstance().Play(SoundManager::SRC::SPRING_SE, SoundManager::PLAYTYPE::BACK);
		action_.SetJumpDecel(SPRING_JUMP_POW);
		action_.ChangeAction(PlayerAction::ATK_ACT::JUMP);
	}
}

void PlayerOnHit::ColGoal(const std::weak_ptr<Collider> _hitCol)
{
	HitModelCommon(_hitCol);
	if (!isSide_&&isLandHit_)
	{
		isGoal_ = true;
	}
}
void PlayerOnHit::CollCoin(void)
{
	coinNum_++;

	//マルチの時のみ処理を行う
	if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::MULTI)
	{
		//スコアの格納
		ScoreManager::GetInstance().AddScore(0, ScoreManager::SCORE_TYPE::COIN);
	}

}
#ifdef DEBUG_ON
void PlayerOnHit::DrawDebug(void)
{
	//colParam_[BODY_SPHERE_COL_NO].geometry_->Draw();
	//colParam_[MOVE_LINE_COL_NO].geometry_->Draw();
	//colParam_[UP_AND_DOWN_LINE_COL_NO].geometry_->Draw();
	//colParam_[EYE_LINE_NO].geometry_->Draw();


	if (action_.GetIsHitPunch())
	{
		colParam_[HAND_SPHERE_COL_NO].geometry_->Draw();
	}
}
#endif // DEBUG_ON



void PlayerOnHit::HitModelCommon(const std::weak_ptr<Collider> _hitCol)
{
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	//当たったモデルの情報を取得
	//移動後座標と現在座標で早い移動速度でも対応させる
	VECTOR hitPos = hitModel.GetHitLineInfo().HitPosition;
	//移動後と移動前のコライダ
	auto& moveLineCol = colParam_[MOVE_LINE_COL_NO].collider_;
	//上下を引いたラインのコライダ(接地)
	auto& upDownLine = colParam_[UP_AND_DOWN_LINE_COL_NO].collider_;
	//球の当たり判定(プレイヤーの周囲)
	auto& bodyShere = colParam_[BODY_SPHERE_COL_NO].collider_;


	//アクションに渡すフラグの初期化
	isLandHit_ = false;
	isHitSlimeFloor_ = false;


	if (moveLineCol->IsHit())
	{
		//Y座標のみ半径分上に移動させる
		movedPos_.y = hitPos.y + Player::RADIUS + POSITION_OFFSET;
		action_.SetJumpPow(Utility::VECTOR_ZERO);
		action_.SetIsJump(false);

		//現在座標の更新
		trans_.pos = movedPos_;
		return;
	}
	//プレイヤーの接地
	if (upDownLine->IsHit())
	{
		Line& upDown = dynamic_cast<Line&>(upDownLine->GetGeometry());
		Collider::TAG tag = upDownLine->GetTags()[0];
		VECTOR hitLinePos = upDown.GetHitInfo().HitPosition;
		if (movedPos_.y > hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y + Player::RADIUS + POSITION_OFFSET;
			//地面と当たっている
			isLandHit_ = true;
			action_.SetJumpPow(Utility::VECTOR_ZERO);
		}
		else
		{
			movedPos_.y = hitLinePos.y - Player::RADIUS - POSITION_OFFSET;
			if (action_.GetJumpDecel() > 0.0f)
			{
				//オブジェクトの下に当たったら跳ね返るようにする
				action_.SetJumpDecel(DOWN_BOUNCE_DECELERATION);
			}
		}
	}
	Collider::TAG tag = _hitCol.lock()->GetTags()[0];
	//移動後座標を一回格納し、移動前をとる
	Transform trans = Transform(trans_);
	trans.pos = movedPos_;
	trans.Update();
	isSide_ = false;
	//プレイヤーの体の球が当たったら
	if (bodyShere->IsHit())
	{
		auto& hitInfo = hitModel.GetHitInfo();
		for (int i = 0; i < hitInfo.HitNum; i++)
		{
			auto hit = hitInfo.Dim[i];
			VECTOR hitPos = hit.HitPosition;
			for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
			{
				int pHit = HitCheck_Sphere_Triangle(trans.pos, RADIUS
					, hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					isSide_ = true;
					VECTOR normal = hit.Normal;
					//y座標を抜いて押しだす
					normal.y = 0.0f;
 					movedPos_ = VAdd(movedPos_, VScale(normal, HIT_NORMAL_OFFSET));
					
					//カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
			
		}
		if(isSide_&&_hitCol.lock()->GetTags()[0]!=Collider::TAG::KILLER_SPECIFIC)
		{
			int modelId = _hitCol.lock()->GetParent().GetTransform().modelId;
			VECTOR pos1 = movedPos_;
			VECTOR pos2 = action_.AddPosRotate(movedPos_, trans_.quaRot, EYE_RANGE);
			//目線がモデルに当たって無ければ上に押し上げる
			if (MV1CollCheck_Line(modelId,-1, pos1,pos2).HitFlag==0)
			{
				if (!Utility::EqualsVZero(action_.GetMovePow()))
				{
					movedPos_.y +=EYE_HEIGHT.y;
				}
				
			}
		}
	}

	////移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;
}
