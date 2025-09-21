#pragma once
#include "../ObjectBase.h"

class Shadow :public ObjectBase
{
public:
	static constexpr int LINE_RANGE = 1000;			//当たり判定の長さ
	static constexpr int RADIUS = 15;				//影の半径
	static constexpr int RADIUS_MAX = 50;			//影の最大半径
	static constexpr int VERTEX_NUM = 36;			//影の頂点数
	static constexpr float ALPHA_MIN = 0.5f;		//影の透明度の最低
	Shadow(Transform& _trans);

	// デストラクタ
	virtual ~Shadow(void);

	//読み込み
	virtual void Load(void) override;
	//初期化
	virtual void Init(void) override;
	//更新d
	virtual void Update(void) override;
	//描画
	virtual void Draw(void) override;

	virtual void OnHit(const std::weak_ptr<Collider> _hitCol) override;

private:
	
	//親トランスフォーム
	Transform& parentTrans_;				
	//影を表示するか
	bool isDraw_;							
	//影の中心位置
	VECTOR pos_;							
	//透明度 0.0 - 1.0
	float alpha_;			
	//当たり判定をしないTag一覧
	std::vector<Collider::TAG> NoneTags_ ;	
	//影の半径
	float radius_;
};

