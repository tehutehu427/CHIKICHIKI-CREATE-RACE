#pragma once
#pragma once
#include <map>
#include <functional>
#include "../../Object/Common/Transform.h"
#include "../ObjectBase.h"

class SkyDome : public ObjectBase
{
public:

	static constexpr float ROT_SPEED = 0.05f;

	static constexpr float SCALE = 100.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// コンストラクタ
	SkyDome();

	// デストラクタ
	~SkyDome(void)override = default;

	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// トランスフォームを返す
	/// </summary>
	/// <returns></returns>トランスフォーム
	const Transform& GetTransform() const;

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

