#pragma once
#include<DxLib.h>
#include<functional>
#include "../../Common/Vector2.h"
class EditController
{
public:
	enum class MODE
	{
		ITEM_SELECT,	//アイテム選択
		MOVE,			//オブジェクト移動
		ROTATE,			//オブジェクト回転
	};

	EditController();
	~EditController() = default;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void ChengeMode(MODE mode);
protected:

private:
	Vector2 mousePos_;	//2Dのマウス座標
	VECTOR mapPos_;	//3Dのマップ座標
	MODE mode_;	//モード
	//モード管理(遷移時の初期処理)
	std::map<MODE, std::function<void(void)>> modeChanges_;
	//モード管理(更新ステップ)
	std::function<void(void)> modeUpdate_;
	//モード管理(更新ステップ)
	std::function<void(void)> modeDraw_;
	void ChengeModeItemSelect(void);
	void ChengeModeMove(void);
	void ChengeModeRotate(void);
	void ItemSelectUpdate(void);	//アイテム選択
	void MoveObjectUpdate(void);	//オブジェクト移動
	void RotateObjectUpdate(void);	//オブジェクト回転
	void ItemSelectDraw(void);		//アイテム選択
	void MoveObjectDraw(void);		//オブジェクト移動
	void RotateObjectDraw(void);	//オブジェクト回転

	void ItemNotSelect(void);		//アイテム選択解除
	IntVector3 NearObjectPos(void);	//近くのオブジェクトの座標を取得
};

