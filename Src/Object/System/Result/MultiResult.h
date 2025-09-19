#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

class MultiParty;
class Palette;
class ScoreGageManager;
class MultiInputCheck;

class MultiResult
{

public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		READY,			//リザルトパレットを開く
		WAIT,			//キャラクター現在のスコアを表示、
		SCORE_UPDATE,	//スコアを更新
		RESULT,			//結果に応じた演出やアニメーションを実行
		INPUT_CHECK,	//入力確認
	};

	//待機時間
	static constexpr float WAIT_TIME = 0.8f;	

	//スコア更新後の待機時間
	static constexpr float AFTER_WAIT_TIME = 1.5f;	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiResult(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiResult(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="_parent">親クラス</param>
	void Update(MultiParty& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset(void);

private:

	//待機用ステップ
	float waitStep_;

	//状態
	STATE state_;

	//状態別更新処理を管理
	std::unordered_map<STATE, std::function<void(MultiParty&)>> stateUpdateMap_;

	//状態別描画処理を管理
	std::unordered_map<STATE, std::function<void(void)>> stateDrawMap_;

	//パレット
	std::unique_ptr<Palette> palette_;

	//スコアゲージ
	std::unique_ptr<ScoreGageManager> scoreGages_;

	//全体入力確認
	std::unique_ptr<MultiInputCheck> inputCheck_;

	//状態別更新処理を登録
	void RegisterStateUpdateFunction(const STATE _state, std::function<void(MultiParty&)> _update);

	//状態別描画処理を登録
	void RegisterStateDrawFunction(const STATE _state, std::function<void(void)> _draw);

	//状態変更
	const void ChangeState(const STATE _state) { state_ = _state; }

	//状態別更新処理
	void UpdateStateReady(MultiParty& _parent);
	void UpdateStateWait(MultiParty& _parent);
	void UpdateStateScore(MultiParty& _parent);
	void UpdateStateResult(MultiParty& _parent);
	void UpdateStateInputCheck(MultiParty& _parent);

	//状態別描画処理
	void DrawStateReady(void) {};
	void DrawStateWait(void);
	void DrawStateScore(void);
	void DrawStateResult(void);
	void DrawStateInputCheck(void);
};