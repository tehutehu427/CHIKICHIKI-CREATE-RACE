#pragma once
#include<functional>
#include<map>
#include "../../../Application.h"
#include "../../../Common/Vector2.h"


class EditerPaletteBase
{
public:

	enum class STATE
	{
		NONE,	
		CLOSE,	//閉じる
		OPEN,	//開ける
		SELECT	//選ぶ処理
	};

	//閉じてる時の位置
	static constexpr int CLOSE_POS_X = 0;
	static constexpr int CLOSE_POS_Y = 0;

	//開いてる時の位置
	static constexpr int OPEN_POS_X = 0;
	static constexpr int OPEN_POS_Y = 0;

	//大きさ
	static constexpr int PALETTE_SIZE_X = 600;
	static constexpr int PALETTE_SIZE_Y = 400;

	EditerPaletteBase();
	~EditerPaletteBase();

	virtual void Load();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	//状態遷移
	void ChangeState(const STATE _state);

private:
	
	//パレットに関する情報
	struct Palette
	{
		int img;
		Vector2 pos;
		Vector2 size;
	};

	//状態変更処理の管理
	std::map<STATE, std::function<void(void)>> stateChanges_;

	//更新処理管理
	std::function<void(void)> stateUpdate_;

	//スクロール用アイコン画像
	int imgScrIcon_;

	//状態
	STATE state_;

	//パレットに関する情報
	Palette pal_;

	//状態変更
	void ChangeStateNone();
	void ChangeStateClose();
	void ChangeStateOpen();
	void ChangeStateSelect();

	//更新処理
	void UpdateNone();
	void UpdateClose();
	void UpdateOpen();
	void UpdateSelect();

};

