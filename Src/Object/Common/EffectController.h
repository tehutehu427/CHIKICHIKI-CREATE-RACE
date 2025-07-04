#pragma once

#include<vector>
#include"../Common/Quaternion.h"

class EffectController
{
public:

	struct Effect
	{
		int playId;			//再生データ
		float speedPer;		//再生スピード割合
		float time;			//再生時間
		float playCount;	//再生カウンタ
	};

	//コンストラクタ
	EffectController(void);

	//デストラクタ
	~EffectController(void);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="_effHandle">エフェクトハンドル</param>
	/// <param name="_pos">座標情報</param>
	/// <param name="_quaRot">回転情報</param>
	/// <param name="_scl">大きさ情報</param>
	/// <param name="speedMultiplier">速度の倍率</param>
	/// <returns>再生したエフェクトの配列番号(このエフェクトの設定をいじる時に使用)</returns>
	const int Play(const int _effHandle,
		const VECTOR _pos,
		const Quaternion _quaRot,
		const VECTOR _scl,
		const float speedMultiplier = NORMAL_MULTI);

	/// <summary>
	/// 再生中エフェクトを止める
	/// </summary>
	/// <param name="_arrayNum">止めたいエフェクトの配列番号</param>
	void Stop(const int _arrayNum);

private:

	static constexpr float NORMAL_MULTI = 1.0f;	//通常倍率

	int effectNum_;					//プレイ中のエフェクトの数
	std::vector<Effect> effects_;	//エフェクト関連
};

