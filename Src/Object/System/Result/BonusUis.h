#pragma once
#include "../../../Common/Vector2.h"


class BonusUis
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_playerIndex">プレイヤー番号</param>
	BonusUis(const int _playerIndex);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BonusUis();

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
	void Update();

	/// <summary>
	/// UIを揺らす処理
	/// </summary>
	void UpdateShake();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ボーナス設定
	/// </summary>
	void SetBonus();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:

	//拡大率最大
	static constexpr float RATE_MAX = 0.5f;

	//拡大率最小
	static constexpr float RATE_MIN = 0.0f;

	//通常座標
	static constexpr int POS_X = 780;
	static constexpr int POS_Y = 205;

	//コインのアニメーションスタート時間
	static constexpr int START_COIN_TIME = 1;

	static constexpr float ALPHA_SPEED = 5.0f;


	/// <summary>
	/// 種類
	/// </summary>
	enum class TYPE
	{
		CLEAR,
		FIRST,
		COIN,
		KILL,
		MAX
	};

	//UI情報
	struct Info
	{
		int image;
		Vector2 pos;
		float rate;
		float alpha;
		bool isDisplay;
	};

	//プレイヤー番号
	int playerIndex_;

	//タイマー
	float timer_;

	//uiの情報を格納
	Info uiInfos_[static_cast<int>(TYPE::MAX)];

};

