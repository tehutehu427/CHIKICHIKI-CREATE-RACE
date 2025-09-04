#pragma once
#include<DxLib.h>
#include"../Common/Transform.h"

class RoundDisplay
{
public:

	//カメラの俯瞰位置
	static constexpr VECTOR CAMERA_OVERLOOKING_POS = { 0.0f,2000.0f,3500.0f };
	static constexpr float SPEED = 10.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RoundDisplay();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RoundDisplay();

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
	/// ナンバーインデックスに値を追加
	/// </summary>
	/// <param name="_addIndex">追加値</param>
	void AddNumberIndex(const int _addIndex);

	/// <summary>
	/// ナンバーを返す
	/// </summary>
	/// <returns>ナンバーを返す</returns>
	const int GetNumberIndex()const { return numberIndex_; }

	//カメラ俯瞰の処理
	void CameraOverLooking(void);

	//カウンタ初期化
	void CountReset(void) { cnt_ = 0.0f; }

private:

	//ラウンド画像
	int imgRound_;

	//ナンバー画像
	int* imgNumbers_;

	//ナンバー用インデックス
	int numberIndex_;

	//ナンバーの描画桁数
	int numberDigit_;

	//カメラ位置
	VECTOR cameraPos_;

	//カウンタ
	float cnt_;
};

