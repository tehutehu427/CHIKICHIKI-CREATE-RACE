#pragma once

class EffectManager
{
public:

	//パーティクル
	static constexpr int PARTICLE_MAX = 8000;	//パーティクルの最大表示数

	//インスタンスの初期生成
	void CreateInstance(void);

	//インスタンスの取得
	static EffectManager& GetInstance(void) { return *instance_; };

	//解放
	void Destroy(void);

private:

	//インスタンス
	static EffectManager* instance_;

	//コンストラクタ
	EffectManager(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>false:エフェクシアの初期化失敗のためプログラムを終了</returns>
	const bool Init(void);

	//コピーの禁止
	EffectManager(const EffectManager& _copy) = delete;
	EffectManager& operator=(const EffectManager& _copy) = delete;
	
	//デストラクタ
	~EffectManager(void);
};

