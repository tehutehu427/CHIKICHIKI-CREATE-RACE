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
	NoneTags_.push_back(Collider::TAG::WIND);

	//影の当たり判定
	std::unique_ptr<Line>lineGeo = std::make_unique<Line>(parentTrans_.pos, parentTrans_.quaRot, VECTOR(), VECTOR(0.0f,-LINE_RANGE,0.0f));
	MakeCollider({ Collider::TAG::SHADOW }, std::move(lineGeo),NoneTags_);
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
		VECTOR vec1 = Utility::VNormalize({ std::cos(angleRad * i), 0.0f, std::sin(angleRad * i) });
		VECTOR vec2 = Utility::VNormalize({ std::cos(angleRad * (i + 1)), 0.0f, std::sin(angleRad * (i + 1)) });
		VECTOR pos1 = pos_;
		VECTOR pos2 = VAdd(pos_, VScale(vec1, radius_));
		VECTOR pos3 = VAdd(pos_, VScale(vec2, radius_));
 		int ret = DrawTriangle3D(pos1, pos3, pos2, Utility::BLACK, true);
 		//ret = DrawTriangle3D(VGet(0.0f,0.0f,0.0f), VGet(500.0f,100.0f,100.0f), VGet(250.0f,0.0f,0.0f), Utility::BLACK, true);
 		ret = ret;
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
		pos_.y += 0.5f; //少し上に表示する
		alpha_ =1.0 - (abs(VSub(parentTrans_.pos, pos_).y)/ LINE_RANGE); //親の位置からの距離で透明度を変える
		radius_ = RADIUS_MAX - ( alpha_ * (RADIUS_MAX - RADIUS)) + RADIUS; //透明度で半径を変える - RADIUS_MAX) + 
	}
}
