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

	//コンストラクタ
	EditController();

	//デストラクタ
	~EditController() = default;

	void Init(void);		//初期化
	void Update(void);		//更新
	void Draw(void);		//描画

	//モード変更
	void ChengeMode(MODE mode);

protected:

private:
	//2Dのマウス座標
	Vector2 mousePos_;	

	//3Dのマップ座標
	VECTOR mapPos_;	

	//モード
	MODE mode_;		

	//オブジェクトをクリックしたか
	bool isClickObject_;	

	//モード管理(遷移時の初期処理)
	std::map<MODE, std::function<void(void)>> modeChanges_;

	//モード管理(更新ステップ)
	std::function<void(void)> modeUpdate_;

	//モード管理(更新ステップ)
	std::function<void(void)> modeDraw_;

	//モード別変更処理
	void ChengeModeItemSelect(void);
	void ChengeModeMove(void);
	void ChengeModeRotate(void);

	//モード別更新処理
	void ItemSelectUpdate(void);	//アイテム選択
	void MoveObjectUpdate(void);	//オブジェクト移動
	void RotateObjectUpdate(void);	//オブジェクト回転

	//モード別描画処理
	void ItemSelectDraw(void);		//アイテム選択
	void MoveObjectDraw(void);		//オブジェクト移動
	void RotateObjectDraw(void);	//オブジェクト回転

	//アイテム選択解除
	void ItemNotSelect(void);		
	
	//近くのオブジェクトの座標を取得
	IntVector3 NearObjectPos(void);	
};

