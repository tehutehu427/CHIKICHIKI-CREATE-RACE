#pragma once
#include "../../../Common/Vector2.h"
#include "../../../Manager/System/InputManager.h"

class PaletteCursor
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_plNum"></param>
	/// <param name="_img"></param>
	PaletteCursor(const int _plNum, const int _img);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PaletteCursor(void) = default;

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
	/// 決定の設定
	/// </summary>
	/// <param name="_decide"></param>決定の判定
	inline const void SetDecide(const bool _decide) { decide_ = _decide; }

	/// <summary>
	/// 座標を返す
	/// </summary>
	/// <returns></returns>座標
	inline const Vector2 GetPos(void) const { return pos_; }

	/// <summary>
	/// 決定済みか返す
	/// </summary>
	/// <returns></returns>結果
	inline const bool IsDecide(void) const { return decide_; }

private:

	//初期位置
	static constexpr int DEFAULT_POS_X = 150;
	static constexpr int DEFAULT_POS_Y = 150;

	//プレイヤー最大人数
	static constexpr int NUM_MAX = 4;

	//移動量
	static constexpr int MOVE_POW = 14;

	//半径
	static constexpr int RADIUS = 32;

	//角度
	static constexpr float ANGLE = 315.0f;

	//拡大率
	static constexpr float RATE_SCALE = 1.0f;

	//プレイヤーナンバー
	int playerNum_;

	//画像
	int img_;

	//拡大率
	float rate_;

	//座標
	Vector2 pos_;

	//決定
	bool decide_;

	//パッド情報
	KeyConfig::JOYPAD_NO pad_;
};

