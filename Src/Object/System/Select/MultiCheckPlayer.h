#pragma once
#include <functional>
#include <unordered_map>
#include "../../../Common/Vector2.h"
#include "../../Common/Transform.h"

class AnimationController;
class Animation2DController;
class ToonStyle;

class MultiCheckPlayer
{
public:

	//アニメーションインデックス
	static constexpr int ANIM_INDEX_IDLE = 1;
	static constexpr int ANIM_INDEX_PUNCH = 12;
	static constexpr int ANIM_INDEX_WAVE_HAND = 6;

	//状態
	enum class STATE
	{
		SMOKE,		//登場は煙エフェクトを出す
		ANIMATION,	//アニメーションを行う
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiCheckPlayer();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiCheckPlayer();

	/// <summary>
	/// 生成
	/// </summary>
	void Create(const int _playerIndex, const int _playerNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 状態変更処理
	/// </summary>
	/// <param name="_state">変更先の状態</param>
	void ChangeState(const STATE _state) { state_ = _state; }

	/// <summary>
	/// ラウンド毎にリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// ゲーム開始時のアニメーションを設定
	/// </summary>
	void SetGameStartAnimation();

	/// <summary>
	/// ゲーム開始時のアニメーションが終了したら返す
	/// </summary>
	bool IsFinishGameStartAnimation();

private:

	//アニメーション停止ステップ
	static constexpr float ANIM_STOP_STEP = 30.0f;

	//エフェクトスピード
	static constexpr float EFFECT_SPEED = 0.5f;

	//プレイヤー番号
	int playerIndex_;

	//プレイヤー人数
	int playerNum_;

	//煙スプライト
	int* imgSmokes_;

	//状態
	STATE state_;

	//プレイヤーのスクリーン座標
	Vector2 screenPos_;

	//プレイヤーのトランス
	Transform trans_;

	//アニメーションコントローラー
	std::unique_ptr<AnimationController> animController_;

	//エフェクト用2Dアニメーションコントローラー
	std::unique_ptr<Animation2DController> effectController_;

	//トゥーン
	std::unique_ptr<ToonStyle> toon_;

	//状態別更新処理の管理
	std::unordered_map<STATE, std::function<void()>> stateMap_;

	//状態別処理の登録
	void RegisterStateUpdate(const STATE _state, std::function<void()>_update);

	//状態別更新
	void UpdateSmoke();
	void UpdateAnimation();

	//プレイヤー番号別設定
	void SetParamByPlayerIndex();

};

