#pragma once
#include "ScoreGage.h"
#include <memory>
#include <vector>

class ScoreGage;

class ScoreGageManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ScoreGageManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ScoreGageManager();

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
	/// 状態遷移
	/// </summary>
	/// <param name="_state">状態を変更</param>
	void ChangeAllState(const ScoreGage::STATE _state);

	/// <summary>
	/// すべてのゲージのアニメーション終了判定
	/// </summary>
	/// <returns>全てのゲージがアニメーションを終えたらtrue, そうでなければfalse</returns>
	const bool IsFinishAnimation() const;

	//装飾類の描画
	void DrawGageDecoration();

	//ボタンを押してねの描画
	void DrawPushButton();

	//タイトルの描画
	void DrawTitle();

	//勝っているかの描画
	void DrawIsWinning();

private:
	// タイトル画像
	int imgTitle_;

	// ボタンを押してね画像
	int imgPushButton_; 

	//が勝っている画像
	int imgIsWinning_;

	//プレイヤープレート
	int* imgPlayers_;

	//メッセージのアルファ値
	int mesAlpha_;

	//アルファ値の変化方向
	int alphaDir_;

	// スコアゲージのリスト
	std::vector<std::unique_ptr<ScoreGage>> scoreGages_; 

};

