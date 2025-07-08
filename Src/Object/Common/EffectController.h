#pragma once

#include<vector>
#include<map>
#include"../Common/Quaternion.h"

class EffectController
{
public:

	//エフェクトの種類
	enum class EFF_TYPE
	{
		NONE,
		WIND,		//風
	};

	//コンストラクタ
	EffectController(void);

	//デストラクタ
	~EffectController(void);

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="_effHandle">エフェクトハンドル</param>
	/// <param name="_type">ハンドルを保存したいタイプ</param>
	void Add(const int _effHandle, const EFF_TYPE _effType);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_pos">座標情報</param>
	/// <param name="_quaRot">回転情報</param>
	/// <param name="_scl">大きさ情報</param>
	/// <param name="speedMultiplier">速度の倍率</param>
	/// <returns>再生したエフェクトの配列番号(このエフェクトの設定をいじる時に使用), -1:失敗</returns>
	const int Play(const EFF_TYPE _effType,
		const VECTOR _pos,
		const Quaternion _quaRot,
		const VECTOR _scl,
		const float _speedMultiplier = NORMAL_MULTI);

	/// <summary>
	/// 座標の再設定
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_arrayNum">変更したいエフェクトの配列番号</param>
	/// <param name="_pos">変更後の座標情報</param>
	void SetPos(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _pos);
	
	/// <summary>
	/// 回転の再設定
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_arrayNum">変更したいエフェクトの配列番号</param>
	/// <param name="_quaRot">変更後の回転情報</param>
	void SetQuaRot(const EFF_TYPE _effType, const int _arrayNum, const Quaternion _quaRot);
	
	/// <summary>
	/// 大きさの再設定
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_arrayNum">変更したいエフェクトの配列番号</param>
	/// <param name="_scl">変更後の大きさ</param>
	void SetScale(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _scl);

	/// <summary>
	/// 速度の再設定
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_arrayNum">変更したいエフェクトの配列番号</param>
	/// <param name="_speed">変更後の速度倍率</param>
	void SetSpeed(const EFF_TYPE _effType, const int _arrayNum, const float _speedMultiplier);

	/// <summary>
	/// 再生中エフェクトを止める
	/// </summary>
	/// <param name="_effType">止めたいエフェクトの種類</param>
	/// <param name="_arrayNum">止めたいエフェクトの配列番号</param>
	void Stop(const EFF_TYPE _effType, const int _arrayNum);

	//全停止
	void AllDelete(void);

	/// <summary>
	/// 再生が終わっているか
	/// </summary>
	/// <param name="_effType">確認したいエフェクトの種類</param>
	/// <param name="_arrayNum">確認したいエフェクトの配列番号</param>
	/// <returns>true:再生が終わった</returns>
	const bool IsEnd(const EFF_TYPE _effType, const int _arrayNum);

private:

	//エフェクトデータ
	struct EffectData
	{
		int resId;					//リソースデータ
		std::vector<int> playId;	//再生データ
	};

	static constexpr float NORMAL_MULTI = 1.0f;	//通常倍率

	std::map<EFF_TYPE, int> effectNum_;			//プレイ中のエフェクトの数
	std::map<EFF_TYPE, EffectData> effects_;	//エフェクト関連
};

