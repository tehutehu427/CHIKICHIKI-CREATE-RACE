#pragma once
#include <functional>
#include <unordered_map>
#include "../../Common/Vector2.h"
#include "../../Scene/SceneBase.h"

class KeyConfig;
class YesNoResponder;

class EditEscape
{

public:	
	
	//アイコンのサイズ
	static constexpr int ICON_SIZE_X = 128;
	static constexpr int ICON_SIZE_Y = 64;

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		WAIT,
		CHECK,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_padCursorPos">パッドのカーソル位置</param>
	EditEscape(const Vector2& _padCursorPos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EditEscape(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 編集可能か返す
	/// </summary>
	/// <returns>trueなら編集可、falseのときは不可</returns>
	const bool IsEdit(void) const;

private:

	//フォントサイズ
	static constexpr int FONT_SIZE = 48;

	//システムメッセージ画像インデックス
	static constexpr int SYS_MES_INDEX = 2;

	//パッドのカーソル位置
	const Vector2& padCursorPos_;

	//キー管理
	KeyConfig& key_;

	//アイコン画像
	int imgIcon_;
	int* imgSystemMessages_;
	
	//フォント
	int font_;

	//アイコン位置
	Vector2 pos_;

	//状態
	STATE state_;

	//回答の返答
	std::unique_ptr<YesNoResponder> responder_;

	//状態管理
	std::unordered_map < STATE, SceneBase::ProcessFunction > stateFunc_;

	//状態変更
	void ChangeState(const STATE _state) { state_ = _state; }

	//処理の登録
	void RegisterStateFunction(const STATE _state, SceneBase::ProcessFunction _func);

	//状態別更新処理
	void UpdateWait(void);
	void UpdateCheck(void);

	//状態別描画処理
	void DrawWait(void);
	void DrawCheck(void);

};

