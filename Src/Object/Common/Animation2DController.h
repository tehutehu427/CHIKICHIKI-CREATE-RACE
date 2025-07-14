#pragma once
#include "../../Common/Vector2.h"

class Animation2DController
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Animation2DController();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Animation2DController();

	/// <summary>
	/// アニメーションパラメーターの設定
	/// </summary>
	/// <param name="startIndex">アニメーションスタートインデックス</param>
	/// <param name="finishIndex">アニメーション終了インデックス</param>
	/// <param name="animSpeed">アニメーションスピード</param>
	/// <param name="isLoop">ループ再生させるか</param>
	void SetAnimParam(const int startIndex, const int finishIndex, const float animSpeed, const bool isLoop = false);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();	
	
	/// <summary>
	/// アニメーションの開始
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// 再生判定
	/// </summary>
	/// <returns>再生中か判定を返す</returns>
	const bool IsPlay()const { return isPlay_; }

	/// <summary>
	/// アニメーションインデックスを返す
	/// </summary>
	/// <returns>アニメーションインデックス</returns>
	const int GetAnimationIndex()const { return animIndex_; }

private:

	//アニメーション開始インデックス
	int startIndex_;

	//アニメーション終了インデックス
	int finishIndex_;

	//アニメーションインデックス
	int animIndex_;

	//アニメーションステップ
	float animStep_;

	//アニメーションスピード
	float animSpeed_;

	//ループを行うか
	bool isLoop_;

	//アニメーション再生
	bool isPlay_;

};

