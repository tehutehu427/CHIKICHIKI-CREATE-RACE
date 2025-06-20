#pragma once
#include "../../../Common/Vector2.h"
#include <functional>
#include <unordered_map>

class Palette
{
public:

	enum class STATE
	{
		NONE,		//非表示
		EXPANSION,	//拡大
		REDUCTION,	//縮小
		ADMISSION,	//入場
		ADMISSION_EDGE,	//画面端手前から入場
		EXIT,		//退場
		EXIT_EDGE,	//画面端手前へ退場
		CENTER,		//中央表示
		EDGE,		//端表示
	};

	//拡大率
	static constexpr float RATE_MAX = 1.8f;

	//拡大速度
	static constexpr float RATE_SPEED = 3.0f;

	//移動速度
	static constexpr float MOVE_SPEED = 800.0f;

	//画面端用オフセット
	static constexpr int EDGE_OFFSET = 250;

	//大きさ
	static constexpr int SIZE_X = 399;
	static constexpr int SIZE_Y = 282;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Palette();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Palette();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="_state"></param>変更先の状態
	void ChangeState(const STATE _state);

	/// <summary>
	/// 座標がパレットの範囲内に含まれてるか返す
	/// </summary>
	/// <param name="_pos">調べる座標</param>
	/// <returns>範囲内ならtrue,そうでない場合false</returns>
	bool IsInsidePalette(const Vector2& _pos);

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <returns>状態</returns>
	inline const STATE GetState()const { return state_; }

private:

	//状態
	STATE state_;

	//処理終了後の遷移状態
	STATE nextState_;

	//画像
	int imgPalette_;

	//拡大率
	float rate_;

	//アニメーション用タイム
	float time_;	
	
	//座標
	Vector2 pos_;

	//開始位置
	Vector2 startPos_;

	//終了位置
	Vector2 finishPos_;

	//更新処理管理
	std::function<void()> stateUpdate_;

	//状態ごとに処理を実行
	std::unordered_map < STATE, std::function<void()>> stateChanges_;

	//状態別変更処理
	void ChangeStateNone();
	void ChangeStateExpansion();
	void ChangeStateReduction();
	void ChangeStateAdmission();
	void ChangeStateAdmissionEdge();
	void ChangeStateExit();
	void ChangeStateExitEdge();
	void ChangeStateCenter();
	void ChangeStateEdge();

	//状態別更新処理
	void UpdateStateNone();
	void UpdateStateExpansion();
	void UpdateStateReduction();
	void UpdateStateAdmission();
	void UpdateStateExit();

	//移動処理
	void Move();

};

