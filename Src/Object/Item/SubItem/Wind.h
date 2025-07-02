#pragma once
#include "../ItemBase.h"
class Wind : public ItemBase
{
public:

	//風の強さ
	static constexpr float POW = 10.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">親の座標</param>
	/// <param name="_quaRot">親の回転</param>
	/// <param name="_scl">親の大きさ</param>
	/// <param name="_size">親のサイズ</param>
	Wind(const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl, const VECTOR _size);
	//デストラクタ
	~Wind()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	VECTOR size_;	//サイズ
	float radius_;	//半径

	VECTOR parentPos_;	//親座標
};

