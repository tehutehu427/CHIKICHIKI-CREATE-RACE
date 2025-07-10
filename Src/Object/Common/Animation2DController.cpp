#include "Animation2DController.h"
#include <DxLib.h>

Animation2DController::Animation2DController()
{
	animIndex_ = -1;
	startIndex_ = -1;
	finishIndex_ = -1;
	animStep_ = 0.0f;
	animSpeed_ = 0.0f;
	isLoop_ = false;
	isPlay_ = false;
}

Animation2DController::~Animation2DController()
{
}

void Animation2DController::SetAnimParam(const int startIndex, const int finishIndex, const float animSpeed, const bool isLoop)
{
	//設定する値が変わってない場合
	if (startIndex_ == startIndex && 
		finishIndex_ == finishIndex && 
		animSpeed_ == animSpeed)
	{
		return; 
	}

	//設定
	startIndex_ = startIndex;
	finishIndex_ = finishIndex;
	animSpeed_ = animSpeed;
	isLoop_ = isLoop;

	//初期化
	animIndex_ = startIndex_;
}

void Animation2DController::Update()
{	
	//アニメーションが非再生の場合
	if (!isPlay_) { return; }	
	
	//アニメーション終了かつループを行わない場合
	if (animIndex_ == finishIndex_ && !isLoop_)
	{
		isPlay_ = false;	//再生しない
		animStep_ = 0.0f;	//初期化
		return;				//処理終了
	}

	//ステップ更新
	animStep_+= animSpeed_;

	//アニメーション最大値を入手
	int animMax = finishIndex_ + 1 - startIndex_;

	//アニメーション設定
	animIndex_ = startIndex_ + static_cast<int>(animStep_) % animMax;
}