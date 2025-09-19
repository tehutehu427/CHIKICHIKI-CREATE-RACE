#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include "../../../Common/Vector2.h"

class BonusUis;

class ScoreGage
{

public:

	enum class STATE
	{
		NONE,		//非表示
		WAIT,		//待機
		ANIMATION,	//アニメーションを実行
		AFTER_WAIT,	//アニメーション後の待機

	};

	//ゲージYサイズ	
	static constexpr int GAGE_SIZE_X = 75;	//ゲージのデフォルトサイズ
	static constexpr int GAGE_SIZE_Y = 50;	//ゲージの太さ

	static constexpr int GAGE_OUTLINE_X = 610;
	static constexpr int GAGE_OUTLINE_Y = 60;

	//ゲージ座標
	static constexpr int GAGE_POS_P1_X = 200;
	static constexpr int GAGE_POS_P1_Y = 180;

	//2P以降の座標をずらす
	static constexpr int GAGE_POS_Y_OFFSET = 100;

	//ゲージ最大
	static constexpr int GAGE_LENGTH_MAX = 500;

	//アニメーション時間
	static constexpr float ANIM_TIME = 3.0f;	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_playerIndex">プレイヤー番号</param>
	ScoreGage(const int _playerIndex);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ScoreGage(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 状態別変更処理
	/// </summary>
	/// <param name="_state">変更先の状態</param>
	void ChangeState(const STATE _state);

	/// <summary>
	/// 1スコア当たりの長さを設定
	/// </summary>
	/// <param name="_lengthPerPoint"></param>
	void SetLengthPerPoint(const int _lengthPerPoint);

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <returns>状態</returns>
	const STATE GetState(void) const { return state_; }

private:

	//ゲージ画像
	int* imgGages_;

	//ゲージ輪郭線
	int imgGageOutline_;

	// プレイヤー番号
	int playerIndex_;

	//スコア最大値
	int scoreMax_;

	// 座標
	Vector2 pos_;

	//サイズ
	Vector2 size_;

	//更新長さ
	int updateLength_;

	// スコアゲージの画像ID
	int imgScoreGage_;
	
	//1スコア当たりの長さ
	float lengthPerPoint_;

	//アニメーション用ステップ
	float animStep_;

	//現在の状態
	STATE state_;

	//状態ごとに処理を登録
	std::unordered_map<STATE, std::function<void()>> stateChanges_;

	//状態別に処理
	std::function<void(void)> stateUpdate_;

	std::unique_ptr<BonusUis> bonus_;

	//状態変更処理
	void ChangeStateNone(void);
	void ChangeStateWait(void);
	void ChangeStateAnimation(void);
	void ChangeStateAfterWait(void);

	//状態別更新処理
	void UpdateStateNone(void) {};
	void UpdateStateAnimation(void);
	void UpdateStateAfterWait(void);

	//プレイヤーの番号ごとにパラメーターを割り当て
	void SetParamToPlayerNo(void);

	//ゲージアニメーション
	void GageAnimation(void) {};

	//ボーナスアニメーション
	void BonusAnimation(void) {};

};

