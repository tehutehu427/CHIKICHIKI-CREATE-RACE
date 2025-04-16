#pragma once
#include "ItemBase.h"
class Floor : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };

	//コンストラクタ
	Floor();
	//デストラクタ
	~Floor()override;

	//読み込み
	void Load(void)override;
	//初期化
	void Init(void)override;
	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//解放
	void Release(void)override;

private:
};

