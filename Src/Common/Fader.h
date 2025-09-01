#pragma once

class Fader
{

public:

	// フェードが進む速さ
	static constexpr float SPEED_ALPHA = 5.0f;

	// 状態
	enum class STATE
	{
		NONE, 
		FADE_OUT,	// 徐々に暗転
		FADE_IN		// 徐々に明転
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Fader();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fader();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void);		
	
	/// <summary>
	/// 指定フェードを開始する
	/// </summary>
	/// <param name="state">フェード状態</param>
	void SetFade(STATE state);
	
	/// <summary>
	/// 状態の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>状態を返す</returns>
	const STATE GetState(void) const { return state_; }

	/// <summary>
	/// 終了判定を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>終了判定を返す</returns>
	const bool IsEnd(void) const { return isEnd_; }

private:

	//透過最大値
	static constexpr int ALPHA_MAX = 255;

	// 画面を少し暗くするようのアルファ値
	static constexpr int LITTLE_ALPHA = 150;

	// 画面を少し暗くする時のスピード
	static constexpr float LITTLE_FADE_OUT_SPEED = 2.0f;

	// フェードが進む速さ
	static constexpr float SPEED_SCENE = 5.0f;	//シーン遷移
	static constexpr float SPEED_PHASE = 2.0f;	//フェーズ遷移

	// 状態
	STATE state_;

	// 透明度
	float alpha_;

	//フェード速度
	float speed_;

	// 透明度の指定値用
	float alphaMax_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

	//マスク画像領域
	int tmpScreen_;

	//マスク用画像
	int imgMask_;

	//円が他のくりぬき処理
	void SpriteMask();

};
