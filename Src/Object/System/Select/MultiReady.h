#pragma once
#include<unordered_map>
#include<memory>
#include "../../../Common/Vector2.h"
#include "../../../Scene/SceneBase.h"

class KeyConfig;
class MultiInputCheck;
class SelectScene;
class MultiCheckPlayer;

class MultiReady
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		RULE_SET,	//ルール設定
		NUM_CHECK,	//人数確認
		PAD_CHECK,	//コントローラー確認
		FINAL_CHECK,//最終確認
		PLAYER_ANIM,//プレイヤーアニメーション
	};

	enum class GAME_RULR
	{
		CLEAR_SCORE,
		SKIP,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiReady();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiReady();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="_parent">親クラス</param>
	void Update(SelectScene& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//選べるプレイヤー人数の最小
	static constexpr int PLAYER_NUM_MIN = 2;

	//メッセージ画像拡大率
	static constexpr float MESSAGE_RATE = 0.7f;

	//ナンバー画像の拡大率
	static constexpr float NUMBER_RATE = 2.0f;

	//アイコンの数
	static constexpr int ICON_NUM = 2;

	//クリアスコアの最大値
	static constexpr int CLEAR_SCORE_MAX = 15;


	//入力管理クラス
	KeyConfig& keyConfig_;

	//メッセージ画像
	int* imgMessages_;

	//ナンバー画像
	int* imgNumbers_;

	//選択画像
	int imgSelectIcon_;

	//プレイヤー人数
	int playerNum_;

	//クリアスコア
	int clearScore_;

	//サウンドボリューム
	int soundVolume_;

	//スキップの有無
	bool isSkip_;

	//状態
	STATE state_;

	//入力確認クラス
	std::unique_ptr<MultiInputCheck> multiInputChecks_;

	//ルール項目別右の処理
	std::unordered_map<GAME_RULR, std::function<void()>> ruleRightMap_;

	//ルール項目別左の処理
	std::unordered_map<GAME_RULR, std::function<void()>> ruleLeftMap_;

	//状態別処理の管理
	std::unordered_map<STATE, SceneBase::ProcessFunction> stateTables_;

	//プレイヤー
	std::vector<std::unique_ptr<MultiCheckPlayer>> players_;

	//状態別処理登録
	void RegisterProcessFunc(const STATE _state, SceneBase::ProcessFunction _funcs);

	//状態変更
	const void ChangeState(const STATE _state) { state_ = _state; }

	//状態別更新処理
	void UpdateRuleSet();
	void UpdateNumCheck();
	void UpdatePadCheck();
	void UpdateFinalCheck();
	void UpdatePlayerAnimation();

	//状態別描画処理
	void DrawRuleSet();
	void DrawNumCheck();
	void DrawPadCheck();
	void DrawFinalCheck();

	//メッセージの描画
	void DrawMessage(const int _posX, const int _posY, const int _imgIndex_);

};

