#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "EffectController.h"

EffectController::EffectController(void)
{
}

EffectController::~EffectController(void)
{
	//エフェクトのクリア
	effects_.clear();
	effectNum_.clear();
}

void EffectController::Add(const int _effHandle, const EFF_TYPE _type)
{
	//すでに入ってるなら何もしない
	if (effects_[_type].resId != -1)return;

	//エフェクト
	EffectData effect;

	//リソースID
	effect.resId = _effHandle;

	//追加
	effects_.emplace(_type, effect);
	effectNum_.emplace(_type, 0);
}

const int EffectController::Play(const EFF_TYPE _effType, const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl, const float _speedMultiplier)
{
	//中身がないなら失敗
	if (effects_[_effType].resId == -1)return -1;

	//プレイハンドルを設定
	int playId = PlayEffekseer3DEffect(effects_[_effType].resId);

	//以降、再生するエフェクトの制御は必ずプレイハンドルIDを使用すること
	//エフェクトの大きさ設定(XYZ)
	SetScalePlayingEffekseer3DEffect(playId, _scl.x, _scl.y, _scl.z);

	//エフェクトの角度を設定
	SetRotationPlayingEffekseer3DEffect(playId, _quaRot.ToEuler().x, _quaRot.ToEuler().y, _quaRot.ToEuler().z);

	//エフェクトの位置を設定
	SetPosPlayingEffekseer3DEffect(playId, _pos.x, _pos.y, _pos.z);	

	//エフェクトの速度を設定
	effects_[_effType].normalSpeed = GetSpeedPlayingEffekseer3DEffect(playId);
	SetSpeedPlayingEffekseer3DEffect(playId, effects_[_effType].normalSpeed * _speedMultiplier);

	//エフェクトの保存
	effects_[_effType].playId.emplace_back(playId);

	//配列保存
	int ret = effectNum_[_effType];

	//カウンタ増加
	effectNum_[_effType]++;

	//配列番号を返す
	return ret;
}

void EffectController::SetPos(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _pos)
{
	SetPosPlayingEffekseer3DEffect(effects_[_effType].playId[_arrayNum], _pos.x, _pos.y, _pos.z);
}

void EffectController::SetQuaRot(const EFF_TYPE _effType, const int _arrayNum, const Quaternion _quaRot)
{
	SetRotationPlayingEffekseer3DEffect(effects_[_effType].playId[_arrayNum], _quaRot.ToEuler().x, _quaRot.ToEuler().y, _quaRot.ToEuler().z);
}

void EffectController::SetScale(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _scl)
{
	SetScalePlayingEffekseer3DEffect(effects_[_effType].playId[_arrayNum], _scl.x, _scl.y, _scl.z);
}

void EffectController::SetSpeed(const EFF_TYPE _effType, const int _arrayNum, const float _speedMultiplier)
{
	SetSpeedPlayingEffekseer3DEffect(effects_[_effType].playId[_arrayNum], effects_[_effType].normalSpeed * _speedMultiplier);
}

void EffectController::Stop(const EFF_TYPE _effType, const int _arrayNum)
{
	//エフェクトストップ
	StopEffekseer3DEffect(effects_[_effType].playId[_arrayNum]);
}
