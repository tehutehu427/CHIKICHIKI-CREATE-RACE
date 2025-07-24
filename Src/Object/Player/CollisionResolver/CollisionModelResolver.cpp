#include"../Utility/Utility.h"
#include"../Object/ObjectBase.h"
#include"../PlayerAction.h"
#include"../PlayerOnHit.h"
#include"../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "CollisionModelResolver.h"

CollisionModelResolver::CollisionModelResolver(VECTOR& _moveDiff, VECTOR& _currentPos, VECTOR& _movedPos
	,PlayerAction& _action
	, std::vector<ObjectBase::ColParam>& _colParam)
	:CollisionResolverBase(_moveDiff,_currentPos,_movedPos,_action,_colParam)
{
	hitPos_ = Utility::VECTOR_ZERO;
}

CollisionModelResolver::~CollisionModelResolver(void)
{
}

void CollisionModelResolver::Resolve(const std::weak_ptr<Collider> _hitCol)
{
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	hitPos_ = hitModel.GetHitLineInfo().HitPosition;
	HitMoveLine();
	HitUpdownLine();
	HitBodyShere(_hitCol);
	
	//現在座標の更新
	moveDiff_ = currentPos_;
	currentPos_ = movedPos_;
}

void CollisionModelResolver::HitUpdownLine(void)
{
	//上下を引いたラインのコライダ(接地)
	auto& upDownLine = colParam_[UP_AND_DOWN_LINE_COL_NO].collider_;

	//プレイヤーの接地
	if (upDownLine->IsHit())
	{
		Line& upDown = dynamic_cast<Line&>(upDownLine->GetGeometry());
		Collider::TAG tag = upDownLine->GetTags()[0];
		//VECTOR hitLinePos = upDown.GetHitInfo().HitPosition;
		hitPos_ = upDown.GetHitInfo().HitPosition;
		if (movedPos_.y > hitPos_.y)
		{
			movedPos_.y = hitPos_.y + Player::RADIUS + POSITION_OFFSET;
			//地面と当たっている
			//isLandHit_ = true;
			action_.SetJumpPow(Utility::VECTOR_ZERO);
		}
		else
		{
			movedPos_.y = hitPos_.y - Player::RADIUS - POSITION_OFFSET;
			if (action_.GetJumpDecel() > 0.0f)
			{
				//オブジェクトの下に当たったら跳ね返るようにする
				action_.SetJumpDecel(DOWN_BOUNCE_DECELERATION);
			}
		}

	}
}

void CollisionModelResolver::HitMoveLine(void)
{
	//移動ラインのコライダ(接地)
	auto& moveLineCol = colParam_[MOVE_LINE_COL_NO].collider_;
	if (moveLineCol->IsHit())
	{
		//Y座標のみ半径分上に移動させる
		movedPos_.y = hitPos_.y + Player::RADIUS + POSITION_OFFSET;
		action_.SetJumpPow(Utility::VECTOR_ZERO);
		action_.SetIsJump(false);
	}
}

void CollisionModelResolver::HitBodyShere(const std::weak_ptr<Collider> _hitCol)
{
	//球の当たり判定(プレイヤーの周囲)
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	auto& bodyShere = colParam_[BODY_SPHERE_COL_NO].collider_;
	//目線のライン(プレイヤーの目線)
	auto& eyeLine = colParam_[EYE_LINE_NO].collider_;
	//移動後座標を一回格納し、移動前をとる
	//Transform trans = Transform(trans_);
	VECTOR currentPos = movedPos_;
	//trans.Update();
	//isSide_ = false;
	if (bodyShere->IsHit())
	{
		auto& hitInfo = hitModel.GetHitInfo();
		for (int i = 0; i < hitInfo.HitNum; i++)
		{
			auto hit = hitInfo.Dim[i];
			VECTOR hitPos = hit.HitPosition;
			for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
			{
				int pHit = HitCheck_Sphere_Triangle(currentPos, Player::RADIUS
					, hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					//isSide_ = true;
					VECTOR normal = hit.Normal;
					//y座標を抜いて押しだす
					normal.y = 0.0f;
					movedPos_ = VAdd(movedPos_, VScale(normal, HIT_NORMAL_OFFSET));

					//カプセルを移動させる
					currentPos = movedPos_;
					continue;
				}
				break;
			}

		}
		if (/*isSide_ && */_hitCol.lock()->GetTags()[0] != Collider::TAG::KILLER_SPECIFIC)
		{
			Line& eyeLineGeo = dynamic_cast<Line&>(eyeLine->GetGeometry());
			int modelId = _hitCol.lock()->GetParent().GetTransform().modelId;
			VECTOR pos1 = eyeLineGeo.GetPosPoint1();
			VECTOR pos2 = eyeLineGeo.GetPosPoint2();
			if (MV1CollCheck_Line(modelId, -1, pos1, pos2).HitFlag == 0)
			{
				if (!Utility::EqualsVZero(action_.GetMovePow()))
				{
					movedPos_.y += Player::EYE_HEIGHT.y;
				}

			}
		}

		////現在座標の更新
		//moveDiff_ = currentPos_;
		//currentPos_ = movedPos_;
	}
}

