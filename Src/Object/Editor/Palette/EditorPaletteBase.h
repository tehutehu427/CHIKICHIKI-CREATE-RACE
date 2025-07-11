#pragma once
#include<functional>
#include<memory>
#include<vector>
#include<map>
#include "../../../Application.h"
#include "../../../Common/Vector2.h"
#include "../../Item/ItemBase.h"

class EditController;
class PaletteIcon;
class Palette;

class EditorPaletteBase
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,	
		WAIT,	//クリック待ち
		PALETTE_ANIM,
		SELECT	//選ぶ処理
	};

	//パレット移動
	static constexpr int PALETTE_MOVE = 20;		
	
	//大きさ
	static constexpr int PALETTE_SIZE_X = 399;
	static constexpr int PALETTE_SIZE_Y = 282;
	
	//拡大率
	static constexpr float PALETTE_RATE = 1.8f;

	//閉じてる時の位置
	static constexpr int CLOSE_POS_X = Application::SCREEN_SIZE_X + 300;
	static constexpr int CLOSE_POS_Y = Application::SCREEN_HALF_Y;

	//開いてる時の位置
	static constexpr int OPEN_POS_X = Application::SCREEN_HALF_X;
	static constexpr int OPEN_POS_Y = Application::SCREEN_HALF_Y;

	/// <summary>
	/// 画像に関する情報
	/// </summary>
	struct ImgInfo
	{
		int num = 0;
		float rate = 0.0f;
		float angle= 0.0f;
		bool selected = false;
		Vector2 pos = {};
		Vector2 size = {};
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_editControllers"></param>エディットコントローラー
	EditorPaletteBase(std::vector<std::unique_ptr<EditController>>& _editControllers);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EditorPaletteBase();
	
	/// <summary>
	/// 読みこみ
	/// </summary>
	virtual void Load();	

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();	

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();	

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebagDraw();

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="_state"></param>変更先の状態
	void ChangeState(const STATE _state);

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <returns></returns>状態
	const STATE GetState()const { return state_; }

protected:

	//パレット
	std::unique_ptr<Palette> pal_;

	//パレットアイコン
	std::unique_ptr<PaletteIcon> palIcon_;

	//エディットコントローラー(参照)
	std::vector<std::unique_ptr<EditController>>& editControllers_;
	
	//画像
	int imgPalette_;	//パレット	

	//状態
	STATE state_;

private:
	
	//状態変更処理の管理
	std::map<STATE, std::function<void()>> stateChanges_;

	//更新処理管理
	std::function<void()> stateUpdate_;



	//状態変更
	void ChangeStateNone();			//なし
	void ChangeStateWait();			//クリック待ち
	void ChangeStateSelect();		//選ぶ
	void ChangeStatePaletteAnim();	//パレットアニメーション

	//更新処理
	void UpdateNone();					//なし
	virtual void UpdateWait();			//クリック待ち
	virtual void UpdateSelect();		//選ぶ
	virtual void UpdatePaletteAnim();	//パレットアニメーション
};