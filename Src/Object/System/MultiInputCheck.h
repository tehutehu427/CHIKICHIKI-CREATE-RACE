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
	MultiInputCheck();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiInputCheck();

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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();	
	
	/// <summary>
	/// 入力状況のみリセットする
	/// </summary>
	void ResetInput();

	/// <summary>
	/// 全体入力が終えたか返す
	/// </summary>
	/// <returns>全員が終えてたらtrue, そうでなければfalse</returns>
	bool IsAllInput();

private:

	//入力マネージャー
	KeyConfig& key_;

	//各プレイヤーの入力確認構造体
	struct PlayerCheckInput
	{
		//入力確認
		bool isInput = false;

		//UI座標位置
		Vector2 uiPos = {};
	};

	//各プレイヤーの入力確認配列
	std::vector<PlayerCheckInput> players_;

};

