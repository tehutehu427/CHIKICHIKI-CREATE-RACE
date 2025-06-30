#include <cmath>
#include "../Utility/Utility.h"
#include "../Common/Collider.h"
#include "../Common/Geometry/Line.h"
#include "../Common/Geometry/Model.h"
#include "Shadow.h"

Shadow::Shadow(Transform& trans) : parentTrans_(trans)
{
	alpha_ = 1.0f;

	//当たり判定をしないタグを設定
	NoneTags_.push_back(Collider::TAG::PLAYER1);
	NoneTags_.push_back(Collider::TAG::PLAYER2);
	NoneTags_.push_back(Collider::TAG::PLAYER3);
	NoneTags_.push_back(Collider::TAG::PLAYER4);
	NoneTags_.push_back(Collider::TAG::PUNCH);
	NoneTags_.push_back(Collider::TAG::SHADOW);
	NoneTags_.push_back(Collider::TAG::CANNON_AIM);
	NoneTags_.push_back(Collider::TAG::KILLER_ITEM);

	//影の当たり判定
	std::unique_ptr<Line>lineGeo = std::make_unique<Line>(parentTrans_.pos, parentTrans_.quaRot, VECTOR(), VECTOR(0.0f,-LINE_RANGE,0.0f));
	MakeCollider({ Collider::TAG::SHADOW }, std::move(lineGeo));
}

Shadow::~Shadow(void)
{
}

void Shadow::Load(void)
{
}

void Shadow::Init(void)
{
}

void Shadow::Update(void)
{
	alpha_ = 0.0f;
	isDraw_ = false;
}

void Shadow::Draw(void)
{
	if (!isDraw_)
	{
		return;
	}
	//影の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_ * 255.0f));
	float angleDeg = 360.0f / VERTEX_NUM;
	float angleRad = Utility::Deg2RadF(angleDeg);

	for (int i = 0; i < VERTEX_NUM; i++)
	{
		VECTOR vec1 = Utility::VNormalize(VECTOR((std::cos(angleRad * i), 0.0f, std::sin(angleRad* i))));
		VECTOR vec2 = Utility::VNormalize(VECTOR((std::cos(angleRad * (i+ 1)), 0.0f, std::sin(angleRad* (i+1)))));
		VECTOR pos1 = pos_;
		VECTOR pos2 = VAdd(pos_, VScale(vec1, RADIUS));
		VECTOR pos3 = VAdd(pos_, VScale(vec2, RADIUS));
		DrawTriangle3D(pos1, pos2, pos3, Utility::BLACK, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast<int>(alpha_ * 255.0f));
}

void Shadow::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	for (const auto tag : _hitCol.lock()->GetTags())
	{
		if (std::find(NoneTags_.begin(), NoneTags_.end(), tag) != NoneTags_.end())
		{
			//当たり判定をしないタグだった
			continue;
		}
		//当たり判定をする
		isDraw_ = true;
		Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
		pos_ = hitModel.GetHitLineInfo().HitPosition;
		pos_.y += 1.5f; //少し上に表示する
		alpha_ = abs(VSub(parentTrans_.pos, pos_).y)/ LINE_RANGE; //親の位置からの距離で透明度を変える

	}
}
