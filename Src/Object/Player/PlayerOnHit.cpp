#include "../../Object/Common/Geometry/Sphere.h"
#include "../../Object/Common/Geometry/Line.h"
#include"../../Object/Common/Geometry/Model.h"
#include"../../Utility/Utility.h"
#include"./Player.h"
#include "PlayerOnHit.h"

PlayerOnHit::PlayerOnHit(PlayerAction& _action, std::vector<ObjectBase::ColParam>& _colParam, Transform& _trans,VECTOR&_movedPos, VECTOR& _moveDiff):
	action_(_action),
	colParam_(_colParam),
	trans_(_trans),
	movedPos_(_movedPos),
	moveDiff_(_moveDiff)

{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;
	colUpdates_[TAG::START] = [this](const std::weak_ptr<Collider> _hitCol) {CollFloor(_hitCol); };
	colUpdates_[TAG::GOAL] = [this](const std::weak_ptr<Collider> _hitCol) {CollFloor(_hitCol); };
	colUpdates_[TAG::NORMAL_ITEM] = [this](const std::weak_ptr<Collider> _hitCol) {CollFloor(_hitCol); };
	colUpdates_[TAG::KILLER_ITEM] = [this](const std::weak_ptr<Collider> _hitCol) {CollKillerItem(_hitCol); };
	colUpdates_[TAG::MOVE_HORI_FLOOR] = [this](const std::weak_ptr<Collider> _hitCol) {CollMoveFloor(_hitCol); };
	colUpdates_[TAG::MOVE_VER_FLOOR] = [this](const std::weak_ptr<Collider> _hitCol) {CollMoveFloor(_hitCol); };
	colUpdates_[TAG::SLIME_FLOOR] = [this](const std::weak_ptr<Collider> _hitCol) {CollSlimeFloor(_hitCol); };
	colUpdates_[TAG::CANNON_AIM] = [this](const std::weak_ptr<Collider> _hitCol) {CollNone(); };
	colUpdates_[TAG::WIND] = [this](const std::weak_ptr<Collider> _hitCol) {CollWind(_hitCol); };
	colUpdates_[TAG::SPRING] = [this](const std::weak_ptr<Collider> _hitCol) {CollNone(); };

	int playerNum = DateBank::GetInstance().GetPlayerNum();
	for (int i = static_cast<int>(TAG::PLAYER1); i < playerNum; i++)
	{
		//同じタグだったら設定しない
		if ((int)tag_ == i)continue;

		colUpdates_[(TAG)i] = [this](const std::weak_ptr<Collider> _hitCol) {CollNone(); };
	}

}

PlayerOnHit::~PlayerOnHit(void)
{
}

void PlayerOnHit::Init(void)
{
}

void PlayerOnHit::Update(const std::weak_ptr<Collider> _hitCol)
{
	Collider::TAG tag = _hitCol.lock()->GetTag();
	colUpdates_[tag](_hitCol);
}

inline void PlayerOnHit::CollNone(void)
{
	//何もしない
}

void PlayerOnHit::CollFloor(const std::weak_ptr<Collider> _hitCol)
{
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	HitModelCommon(hitModel);
}

void PlayerOnHit::CollMoveFloor(const std::weak_ptr<Collider> _hitCol)
{
	if (_hitCol.lock()->GetTag() == Collider::TAG::MOVE_HORI_FLOOR || _hitCol.lock()->GetTag() == Collider::TAG::MOVE_VER_FLOOR)
	{
		ItemBase& floor = dynamic_cast<ItemBase&>(const_cast<ObjectBase&>(_hitCol.lock()->GetParent()));
		VECTOR movePow = floor.GetMovePow();
		movedPos_ = VAdd(movedPos_, floor.GetMovePow());
	}
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	HitModelCommon(hitModel);
}

void PlayerOnHit::CollSlimeFloor(const std::weak_ptr<Collider> _hitCol)
{
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	HitModelCommon(hitModel);
}

void PlayerOnHit::CollCannon(const std::weak_ptr<Collider> _hitCol)
{
}

void PlayerOnHit::CollKillerItem(const std::weak_ptr<Collider> _hitCol)
{
	isDeath_ = true;
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	HitModelCommon(hitModel);
}

void PlayerOnHit::CollWind(const std::weak_ptr<Collider> _hitCol)
{
	//風オブジェクトから移動量を取得
	ItemBase& wind = dynamic_cast<ItemBase&>(const_cast<ObjectBase&>(_hitCol.lock()->GetParent()));
	action_.SetMovePow(wind.GetMovePow());

	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	HitModelCommon(hitModel);
}

void PlayerOnHit::ColPunch(const std::weak_ptr<Collider> _hitCol)
{
	//パンチ中じゃなければ何もしない
	if (!action_.GetIsHitPunch())return;
	//パンチしたプレイヤーの向いてる方向をセットする
	VECTOR punchedPlayerPos = _hitCol.lock()->GetParent().GetTransform().pos;

	//パンチしたプレイヤーの位置と自分の位置を比較して、
	action_.SetDir(Utility::GetMoveVec(punchedPlayerPos, trans_.pos));

	//ノックバック状態遷移
	action_.ChangeAction(PlayerAction::ATK_ACT::KNOCKBACK);
}

void PlayerOnHit::DrawDebug(void)
{
	colParam_[BODY_SPHERE_COL_NO].geometry_->Draw();
	colParam_[MOVE_LINE_COL_NO].geometry_->Draw();
	colParam_[UP_AND_DOWN_LINE_COL_NO].geometry_->Draw();
	colParam_[HAND_SPHERE_COL_NO].geometry_->Draw();
}

//void PlayerOnHit::PosUpdate(void)
//{
//	VECTOR pow = action_.GetMovePow();
//	movedPos_ = VAdd(trans_.pos, action_.GetMovePow());
//	movedPos_ = VAdd(movedPos_, action_.GetJumpPow());
//
//
//#ifdef DEBUG_ON
//
//	////デバッグ用床の当たり判定
//	//if (CollCube())
//	//{
//	//	movedPos_ = VAdd(movedPos_, cubeMovePos_);
//	//	action_->SetJumpPow(Utility::VECTOR_ZERO);
//	//	movedPos_.y = cube_.upPos.y + RADIUS;
//	//	action_->SetStepJump(0.0f);
//	//	action_->SetIsJump(false);
//	//	action_->SetJumpDecel(POW_JUMP);
//	//}
//	//else
//	//{
//	//	action_->SetIsJump(true);
//	//	//if (jumpPow_.y <= LIMIT_GRAVITY)
//	//	//{
//	//	//	jumpPow_.y = LIMIT_GRAVITY;
//	//	//}
//	//}
//
//#endif // DEBUG_ON
//
//	//移動量ラインの更新
//	VECTOR moveVec = VSub(movedPos_, trans_.pos);
//	if (!Utility::EqualsVZero(moveVec))
//	{
//		Line& moveLine = dynamic_cast<Line&>(colParam_[MOVE_LINE_COL_NO].collider_->GetGeometry());
//		moveLine.SetLocalPosPoint1(Utility::VECTOR_ZERO);
//		moveLine.SetLocalPosPoint2(moveVec);
//	}
//	//移動前の座標を格納する
//	moveDiff_ = trans_.pos;
//	//移動
//	trans_.pos = movedPos_;
//	// 現在座標を起点に移動後座標を決める
//}

void PlayerOnHit::HitModelCommon(Model& _hitModel)
{
	//Y座標のみ半径分上に移動させる
	VECTOR hitPos = _hitModel.GetHitLineInfo().HitPosition;
	auto& moveLineCol = colParam_[MOVE_LINE_COL_NO].collider_;
	auto& upDownLine = colParam_[UP_AND_DOWN_LINE_COL_NO].collider_;
	if (moveLineCol->IsHit() > 0)
	{
		//Y座標のみ半径分上に移動させる
		movedPos_.y = hitPos.y + Player::RADIUS + POSITION_OFFSET;
		action_.SetJumpPow(Utility::VECTOR_ZERO);
		action_.SetIsJump(false);
		return;
	}
	//プレイヤーの接地
	if (upDownLine->IsHit() > 0)
	{
		Line& upDown = dynamic_cast<Line&>(upDownLine->GetGeometry());
		VECTOR hitLinePos = upDown.GetHitInfo().HitPosition;
		if (movedPos_.y > hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y + Player::RADIUS + POSITION_OFFSET;
		}
		else
		{
			movedPos_.y = hitLinePos.y - Player::RADIUS - POSITION_OFFSET;
		}
		action_.SetJumpPow(Utility::VECTOR_ZERO);
	}


	//球の当たり判定(プレイヤーの周囲)
	auto& bodyShere = colParam_[BODY_SPHERE_COL_NO].collider_;
	//移動後座標を一回格納し、移動前をとる
	Transform trans = Transform(trans_);
	trans.pos = movedPos_;
	trans.Update();
	if (bodyShere->IsHit())
	{
		auto& hitInfo = _hitModel.GetHitInfo();

		for (int i = 0; i < hitInfo.HitNum; i++)
		{
			auto hit = hitInfo.Dim[i];
			for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
			{
				int pHit = HitCheck_Sphere_Triangle(trans.pos, RADIUS
					, hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					//カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}
	}
	////移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;
	
}
