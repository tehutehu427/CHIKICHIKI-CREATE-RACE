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

	enum class STATE
	{
		READY,			//リザルトパレットを開く
		WAIT,			//キャラクター現在のスコアを表示、
		SCORE_UPDATE,	//スコアを更新
		RESULT,			//結果に応じた演出やアニメーションを実行
	};

	//待機時間
	static constexpr float WAIT_TIME = 0.8f;	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiResult();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiResult();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(MultiParty& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:

	//待機用ステップ
	float waitStep_;

	//状態
	STATE state_;

	//状態別更新処理を管理
	std::unordered_map<STATE, std::function<void(MultiParty&)>> stateMap_;

	//パレット
	std::unique_ptr<Palette> palette_;

	//スコアゲージ
	std::unique_ptr<ScoreGageManager> scoreGages_;

	//プレイヤー
	//std::unique_ptr<ResultPlayer> player_;

	//全体入力確認
	std::unique_ptr<MultiInputCheck> inputCheck_;

	//状態別更新処理を登録
	void RegisterStateFunction(const STATE _state, std::function<void(MultiParty&)> _update);

	//状態変更
	inline const void ChangeState(const STATE _state) { state_ = _state; }

	//状態別更新処理
	void UpdateStateReady(MultiParty& _parent);
	void UpdateStateWait(MultiParty& _parent);
	void UpdateStateScore(MultiParty& _parent);
	void UpdateStateResult(MultiParty& _parent);

};

