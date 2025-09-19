#pragma once
#include <unordered_map>
#include <functional>
#include "../../Common/Transform.h"

class AnimationController;
class ToonStyle;

class SelectSceneActors
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectSceneActors(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectSceneActors(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);

private:	

	//アニメーション用インデックス
	static constexpr int CHICKEN_ANIM_INDEX_WAVE_HAND = 5;

	//スタート位置
	static constexpr VECTOR START_POS = { 200.0f, -200.0f, 150.0f };

	//チキン位置
	static constexpr VECTOR CHICKEN_POS = { 200.0f,-50.0f,50.0f };

	//スタートからチキンとの距離
	static constexpr float DISTANCE = 50.0f;

	//回転角度DEG
	static constexpr float ROT_DEG = 0.5f;
	
	//アクター種類
	enum class ACTOR
	{
		CHICKEN,
		START,
	};

	//アクターの情報
	struct ActorInfomation
	{
		//シェーダー
		std::unique_ptr<ToonStyle> toonStyle;

		//トランスフォーム
		Transform transform;
	};

	//アクターの種類ごとに情報を管理
	std::unordered_map<ACTOR, ActorInfomation> actorInfos_;

	//チキン用アニメーションコントローラー
	std::unique_ptr<AnimationController> chickenAnimationController_;

};

