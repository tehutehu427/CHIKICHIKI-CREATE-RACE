#pragma once
#include "../ItemBase.h"

class Wind : public ItemBase
{

public:

	//風の強さ
	static constexpr float POW = 1.0f;

	//風自体の大きさ
	static constexpr float SIZE = 10.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">親の座標</param>
	/// <param name="_quaRot">親の回転</param>
	/// <param name="_scl">親の大きさ</param>
	/// <param name="_size">親のサイズ</param>
	Wind(const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl, const VECTOR _size);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Wind(void)override;

	/// <summary>
	/// 個人設定
	/// </summary>
	/// <param name=""></param>
	void SetParam(void)override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void)override;

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	//基本情報
	VECTOR size_;		//サイズ
	float radius_;		//半径
	VECTOR parentPos_;	//親座標
};

