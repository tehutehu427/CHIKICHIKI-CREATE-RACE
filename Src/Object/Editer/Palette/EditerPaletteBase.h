#pragma once
#include<functional>
#include<map>
#include "../../../Application.h"
#include "../../../Common/Vector2.h"
#include "../../Item/ItemBase.h"


class EditerPaletteBase
{
public:

	enum class STATE
	{
		NONE,	
		WAIT,	//クリック待ち
		CLOSE,	//閉じる
		OPEN,	//開ける
		SELECT	//選ぶ処理
	};

	//パレット移動
	static constexpr int PALETTE_MOVE = 10;	
	
	//アイコン数
	static constexpr int ICON_NUM = ItemBase::ITEM_NUM_MAX;
	
	//大きさ
	static constexpr int PALETTE_SIZE_X = 399;
	static constexpr int PALETTE_SIZE_Y = 282;
	static constexpr int ICON_SIZE = 128;
	
	//拡大率
	static constexpr float PALETTE_RATE = 1.8f;
	static constexpr int ICON_RATE = 1.0f;

	//閉じてる時の位置
	static constexpr int CLOSE_POS_X = Application::SCREEN_SIZE_X;
	static constexpr int CLOSE_POS_Y = Application::SCREEN_HALF_Y;

	//開いてる時の位置
	static constexpr int OPEN_POS_X = Application::SCREEN_HALF_X;
	static constexpr int OPEN_POS_Y = Application::SCREEN_HALF_Y;

	//アイコン位置
	static constexpr int LINE = 2;
	static constexpr int COL = 4;
	static constexpr int ICON_POS_X = Application::SCREEN_HALF_X - (ICON_SIZE * ICON_RATE * 2);
	static constexpr int ICON_POS_Y[LINE] = {
	   Application::SCREEN_HALF_Y - static_cast<int>(ICON_SIZE * ICON_RATE * 1.5f),
	   Application::SCREEN_HALF_Y + static_cast<int>(ICON_SIZE * ICON_RATE * 1.5f),
	};

	EditerPaletteBase();
	~EditerPaletteBase();

	virtual void Load();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	//デバッグ用描画
	void DebagDraw();

	//状態遷移
	void ChangeState(const STATE _state);

private:
	
	//パレットに関する情報
	struct Palette
	{
		int img;
		float rate;
		Vector2 pos;
		Vector2 size;
	};

	//アイコンに関する情報
	struct Icon
	{
		int num;
		float rate;
		Vector2 pos;
		Vector2 size;
	};

	//状態変更処理の管理
	std::map<STATE, std::function<void(void)>> stateChanges_;

	//更新処理管理
	std::function<void(void)> stateUpdate_;

	//スクロール用アイコン画像
	int imgScrIcon_;

	//アイコン画像
	//int* imgIcons_;
	int imgIcons_;

	//状態
	STATE state_;

	//パレット
	Palette pal_;

	//アイコン
	//Icon icons_[ICON_NUM];
	Icon ic_;

	//状態変更
	void ChangeStateNone();
	void ChangeStateWait();
	void ChangeStateClose();
	void ChangeStateOpen();
	void ChangeStateSelect();

	//更新処理
	void UpdateNone();
	void UpdateWait();
	void UpdateClose();
	void UpdateOpen();
	void UpdateSelect();
};

