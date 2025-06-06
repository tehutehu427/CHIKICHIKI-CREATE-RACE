#pragma once
class Collider
{

public :

	// 衝突種別
	enum class COL_TAG
	{
		NONE,
		STAGE,
		PLAYER,
		DEATH,
		FLOOR,
		SLIME,
	};

	// コンストラクタ
	Collider(COL_TAG type, int modelId);

	// デストラクタ
	~Collider(void);

	// 衝突種別
	COL_TAG type_;

	// モデルのハンドルID
	int modelId_;

};
