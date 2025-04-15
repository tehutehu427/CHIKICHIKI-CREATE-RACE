#pragma once
#include "Common/Transform.h"
#include "../Common/IntVector3.h"

class ResourceManager;
class SceneManager;

class ObjectBase
{
	//仮

public:

	//ステータス
	struct Status
	{
		bool isBreak;		//破壊可能か(true:破壊可能)
		bool isGravity;		//重力影響を受けるか(true:受ける)
		IntVector3 size;	//大きさ(グリッド基準)
	};

	// コンストラクタ
	ObjectBase(void);

	// デストラクタ
	virtual ~ObjectBase(void);

	virtual void Load(void) = 0;
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//モデル情報の取得
	inline const Transform& GetTransform(void) const { return transform_; }

	//ステータスの取得
	inline const Status GetStatus(void)const { return status_; }

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform transform_;

	//ステータス
	Status status_;
};

