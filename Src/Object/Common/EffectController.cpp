#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "EffectController.h"

EffectController::EffectController(void)
{
	effectNum_ = 0;
}

EffectController::~EffectController(void)
{
	//エフェクトのクリア
	effects_.clear();
}

const int EffectController::Play(const int _effHandle, const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl, const float speedMultiplier)
{
	//エフェクトハンドル
	Effect playEffect;

	//プレイハンドルを設定
	playEffect.playId = PlayEffekseer3DEffect(_effHandle);

	//以降、再生するエフェクトの制御は必ずプレイハンドルIDを使用すること
	//エフェクトの大きさ設定(XYZ)
	SetScalePlayingEffekseer3DEffect(playEffect.playId, _scl.x, _scl.y, _scl.z);

	//エフェクトの角度を設定
	SetRotationPlayingEffekseer3DEffect(playEffect.playId, _quaRot.ToEuler().x, _quaRot.ToEuler().y, _quaRot.ToEuler().z);

	//エフェクトの位置を設定
	SetPosPlayingEffekseer3DEffect(playEffect.playId, _pos.x, _pos.y, _pos.z);	

	//エフェクトの速度を設定
	float normalSpeed = GetSpeedPlayingEffekseer3DEffect(playEffect.playId);
	SetSpeedPlayingEffekseer3DEffect(playEffect.playId, normalSpeed / speedMultiplier);

	//エフェクトの保存
	effects_.emplace_back(playEffect);

	//カウンタ増加
	effectNum_++;
}

void EffectController::Stop(const int _arrayNum)
{
	//エフェクトストップ
	StopEffekseer3DEffect(effects_[_arrayNum].playId);
}
