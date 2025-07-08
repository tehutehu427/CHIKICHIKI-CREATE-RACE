#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include<Effekseer.h>
#include "../../Manager/System/SceneManager.h"
#include "EffectController.h"

EffectController::EffectController(void)
{
}

EffectController::~EffectController(void)
{
	//全停止
	AllDelete();
}

void EffectController::Add(const int _effHandle, const EFF_TYPE _effType)
{
	//エフェクトの要素が存在するか
	if (effects_.count(_effType))
	{
		//既に存在したので何もしない
		return;
	}

	//エフェクト
	EffectData effect;

	//リソースID
	effect.resId = _effHandle;

	//追加
	effects_.emplace(_effType, effect);
	effectNum_.emplace(_effType, 0);
}

const int EffectController::Play(const EFF_TYPE _effType, const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl, const float _speedMultiplier)
{
	//エフェクトの要素が存在するか
	if (!effects_.count(_effType))
	{
		//その要素がなかったので失敗
		return -1;
	}

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
	SetSpeedPlayingEffekseer3DEffect(playId, _speedMultiplier);

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
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effectNum_[_effType] <= 0)
	{
		//その要素がなかった
		return;
	}

	//座標の再設定
	SetPosPlayingEffekseer3DEffect(effects_[_effType].playId[_arrayNum], _pos.x, _pos.y, _pos.z);
}

void EffectController::SetQuaRot(const EFF_TYPE _effType, const int _arrayNum, const Quaternion _quaRot)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effectNum_[_effType] <= 0)
	{
		//その要素がなかった
		return;
	}
	
	//回転の再設定
	SetRotationPlayingEffekseer3DEffect(effects_[_effType].playId[_arrayNum], _quaRot.ToEuler().x, _quaRot.ToEuler().y, _quaRot.ToEuler().z);
}

void EffectController::SetScale(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _scl)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effectNum_[_effType] <= 0)
	{
		//その要素がなかった
		return;
	}
	
	//大きさの再設定
	SetScalePlayingEffekseer3DEffect(effects_[_effType].playId[_arrayNum], _scl.x, _scl.y, _scl.z);
}

void EffectController::SetSpeed(const EFF_TYPE _effType, const int _arrayNum, const float _speedMultiplier)
{
	
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effectNum_[_effType] <= 0)
	{
		//その要素がなかった
		return;
	}
	
	//速度の再設定
	SetSpeedPlayingEffekseer3DEffect(effects_[_effType].playId[_arrayNum], _speedMultiplier);
}

void EffectController::Stop(const EFF_TYPE _effType, const int _arrayNum)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effectNum_[_effType] <= 0)
	{
		//その要素がなかった
		return;
	}
	
	//エフェクトストップ
	StopEffekseer3DEffect(effects_[_effType].playId[_arrayNum]);
}

void EffectController::AllDelete(void)
{
	//全停止
	for (auto effectNum : effectNum_)
	{
		for (int i = 0; i < effectNum.second; i++)
		{
			Stop(effectNum.first, i);
		}
	}
	
	//エフェクトのクリア
	effects_.clear();
	effectNum_.clear();
}

const bool EffectController::IsEnd(const EFF_TYPE _effType, const int _arrayNum)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effectNum_[_effType] <= 0)
	{
		//そもそもその要素がなかった
		return true;
	}

	//再生が終わっているか
	if (IsEffekseer3DEffectPlaying(effects_[_effType].playId[_arrayNum]) == -1)
	{
		//終わっていた
		return true;
	}

	//終わっていない
	return false;
}
