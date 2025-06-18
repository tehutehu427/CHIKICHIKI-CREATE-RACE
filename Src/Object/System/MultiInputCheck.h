#pragma once

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
	/// 全体入力が終えたか返す
	/// </summary>
	/// <returns>全員が終えてたらtrue, そうでなければfalse</returns>
	inline const bool IsAllInput() const { return allInput_; }

private:

	//入力マネージャー
	KeyConfig& input_;

	//すべてのプレイヤーが入力したか
	bool allInput_;

};

