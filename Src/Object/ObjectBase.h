#pragma once
#include "Common/Transform.h"
#include "../Common/IntVector3.h"

class ResourceManager;
class SceneManager;

class ObjectBase
{
public:

	// コンストラクタ
	ObjectBase(void);

	// デストラクタ
	virtual ~ObjectBase(void);

	//読み込み
	virtual void Load(void) = 0;
	//初期化
	virtual void Init(void) = 0;
	//更新
	virtual void Update(void) = 0;
	//描画
	virtual void Draw(void) = 0;

	//モデル情報の取得
	inline const Transform& GetTransform(void) const { return trans_; }

	//サイズの取得
	inline const IntVector3 GetSize(void)const { return size_; }

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform trans_;

	//大きさ(グリッド基準)
	IntVector3 size_;
};

