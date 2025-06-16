#pragma once
#include<unordered_map>
#include<memory>
#include "../../../Common/Vector2.h"
#include "../../../Scene/SceneBase.h"

class InputManager;
class MultiInputCheck;
class SelectScene;

class MultiReady
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NUM_CHECK,	//人数確認
		PAD_CHECK,	//コントローラー確認
		FINAL_CHECK	//最終確認
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiReady();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiReady();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="_parent">親クラス</param>
	void Update(SelectScene& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//入力管理クラス
	InputManager& input_;

	//プレイヤー人数
	int playerNum_;

	//状態
	STATE state_;

	//入力確認クラス
	std::unique_ptr<MultiInputCheck> multiInputChecks_;

	//状態別処理の管理
	std::unordered_map<STATE, SceneBase::ProcessFunction> stateTables_;

	//状態別処理登録
	void RegisterProcessFunc(const STATE _state, SceneBase::ProcessFunction _funcs);

	//状態変更
	inline const void ChangeState(const STATE _state) { state_ = _state; }

	//状態別更新処理
	void UpdateNumCheck();
	void UpdatePadCheck();
	void UpdateFinalCheck();

	//状態別描画処理
	void DrawNumCheck();
	void DrawPadCheck();
	void DrawFinalCheck();

};

