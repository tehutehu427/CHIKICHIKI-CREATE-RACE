#pragma once
#pragma once
#include <map>
#include <functional>
#include "../../Object/Common/Transform.h"
#include "../ObjectBase.h"

class SkyDome : public ObjectBase
{

public:

	//回転速度
	static constexpr float ROT_SPEED = 0.05f;

	//大きさ
	static constexpr float SCALE = 60.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkyDome(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~SkyDome(void)override = default;

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name=""></param>
	void Load(void) override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void) override;

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override {};

private:	

	// 状態
	STATE state_;

	// モデル制御の基本情報
	Transform transform_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;
};

