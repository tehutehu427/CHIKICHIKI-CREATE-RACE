#pragma once
#include<functional>
#include<memory>
#include<map>
#include "../../../Application.h"
#include "../../../Common/Vector2.h"
#include "../../Item/ItemBase.h"

class PaletteIcon;

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
	
	//大きさ
	static constexpr int PALETTE_SIZE_X = 399;
	static constexpr int PALETTE_SIZE_Y = 282;
	
	//拡大率
	static constexpr float PALETTE_RATE = 1.8f;

	//閉じてる時の位置
	static constexpr int CLOSE_POS_X = Application::SCREEN_SIZE_X;
	static constexpr int CLOSE_POS_Y = Application::SCREEN_HALF_Y;

	//開いてる時の位置
	static constexpr int OPEN_POS_X = Application::SCREEN_HALF_X;
	static constexpr int OPEN_POS_Y = Application::SCREEN_HALF_Y;

	//画像描画に関する情報
	struct ImgInfo
	{
		int num_;
		float rate;
		float angle;
		Vector2 pos;
		Vector2 size;
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
	
	//状態変更処理の管理
	std::map<STATE, std::function<void(void)>> stateChanges_;

	//更新処理管理
	std::function<void(void)> stateUpdate_;

	//画像
	int imgPalette_;	//パレット	

	//状態
	STATE state_;

	//パレット
	ImgInfo pal_;

	//パレットアイコン
	std::unique_ptr<PaletteIcon> palIcon_;

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