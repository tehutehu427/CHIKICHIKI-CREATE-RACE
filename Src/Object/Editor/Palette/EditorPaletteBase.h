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
	virtual ~EditorPaletteBase(void);
	
	/// <summary>
	/// 読みこみ
	/// </summary>
	virtual void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(void);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebagDraw(void);

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="_state"></param>変更先の状態
	void ChangeState(const STATE _state);

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <returns></returns>状態
	const STATE GetState(void) const { return state_; }

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

	//音源の読み込み
	void LoadSounds(void);

private:
	
	//状態変更処理の管理
	std::map<STATE, std::function<void()>> stateChanges_;

	//更新処理管理
	std::function<void()> stateUpdate_;

	//状態変更
	void ChangeStateNone(void);			//なし
	void ChangeStateWait(void);			//クリック待ち
	void ChangeStateSelect(void);		//選ぶ
	void ChangeStatePaletteAnim(void);	//パレットアニメーション

	//更新処理
	void UpdateNone(void);					//なし
	virtual void UpdateWait(void);			//クリック待ち
	virtual void UpdateSelect(void);		//選ぶ
	virtual void UpdatePaletteAnim(void);	//パレットアニメーション
};