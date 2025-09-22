#pragma once
#include <vector>
#include "../../Common/Vector2.h"

class KeyConfig;

class MultiInputCheck
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiInputCheck(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiInputCheck(void);

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
	/// リセット
	/// </summary>
	void Reset(void);
	
	/// <summary>
	/// 入力状況のみリセットする
	/// </summary>
	void ResetInput(void);

	//ボタンが押されたか返す
	const bool IsInput(const int _playerIndex) const { return players_[_playerIndex].isProcess; }

	/// <summary>
	/// 全体入力が終えたか返す
	/// </summary>
	/// <returns>全員が終えてたらtrue, そうでなければfalse</returns>
	bool IsAllInput(void);

private:

	//画像サイズ
	static constexpr int SIZE = 128;

	//拡大率
	static constexpr float RATE = 0.75f;	
	
	//各プレイヤーの入力確認構造体
	struct PlayerCheckInput
	{
		bool isInput = false;	//入力確認
		bool isProcess = false;	//処理を実行
		Vector2 uiPos = {};		//UI座標位置
		float rate = 0.0f;		//拡大率
		float step = 0.0f;		//ステップ
	};

	//入力マネージャー
	KeyConfig& key_;

	//画像
	int imgOk_;

	//各プレイヤーの入力確認配列
	std::vector<PlayerCheckInput> players_;
};