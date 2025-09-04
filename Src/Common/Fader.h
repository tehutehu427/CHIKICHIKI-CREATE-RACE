#pragma once
#include <functional>
#include <unordered_map>

class Fader
{

public:

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
	/// <param name="_state">フェード状態</param>
	void SetFade(const STATE _state);
	
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

	//フェード時間
	static constexpr float TOTAL_TIME = 1.0f;

	//最大拡大率
	static constexpr float RATE_MAX = 3.0f;

	// 状態
	STATE state_;

	//時間計測用
	float time_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

	//マスク画像領域
	int tmpScreen_;

	//マスク用画像
	int imgMask_;

	//画像の拡大率
	float rate_;

	//状態別更新処理の管理
	std::unordered_map<STATE, std::function<void(void)>> stateUpdateMap_;

	//処理の登録
	void RegisterStateUpdate(const STATE _state, const std::function<void(void)> _func);

	//状態別更新処理
	void UpdateFadeIn();
	void UpdateFadeOut();
	void UpdateNone() {};

	//円が他のくりぬき処理
	void SpriteMask();

};
