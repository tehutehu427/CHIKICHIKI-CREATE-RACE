#pragma once
#include <unordered_map>
#include "../../Common/Vector2.h"
#include "../../Scene/SceneBase.h"

class KeyConfig;

class ManualTab
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		WAIT,
		DISPLAY,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_padCursolPos">パッドコントローラーの座標</param>
	ManualTab(const Vector2& _padCursolPos = {});

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ManualTab();
	
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
	/// マニュアルが表示中か返す
	/// </summary>
	/// <returns>表示中ならtrue,そうでなければfalse</returns>
	inline const bool IsDisplay()const { return state_ == STATE::DISPLAY; }

private:

	//アイコンサイズ
	static constexpr int ICON_SIZE = 128;
	
	//半径
	static constexpr float RADIUS = 64.0f;

	//パッドのカーソル位置
	const  Vector2& padCursolPos_;

	//入力管理
	KeyConfig& key_;

	//アイコン画像
	int imgIcon_;

	//マニュアル画像
	int imgManual_;

	//状態
	STATE state_;

	//UIの座標位置
	Vector2 uiPos_;

	//状態別処理
	std::unordered_map<STATE, SceneBase::ProcessFunction> stateFunc_;

	//処理の登録
	void RegisterStateFunction(const STATE _state, SceneBase::ProcessFunction _func);

	//状態遷移
	inline void ChangeState(const STATE _state) { state_ = _state; }

	//更新関数
	void UpdateWait();
	void UpdateDisplay();

	//描画関数
	void DrawWait();
	void DrawDisplay();

};