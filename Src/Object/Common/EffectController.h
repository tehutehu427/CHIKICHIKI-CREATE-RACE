#pragma once

class EffectController
{
public:

	struct Effect
	{
		int resId;			//初期データ
		int playId;			//再生データ
		float time;			//再生時間
		float playCount;	//再生カウンタ
		VECTOR pos;			//表示座標
		VECTOR rot;			//回転
		VECTOR scl;			//大きさ
	};

	//コンストラクタ
	EffectController(void);

	//デストラクタ
	~EffectController(void);

	void Add();

	void Play();

	void Stop();

private:

	std::map<int, Effect> effects_;		//エフェクト関連
};

