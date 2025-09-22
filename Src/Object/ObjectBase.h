#pragma once
#include<memory>
#include<vector>
#include "Common/Transform.h"
#include "../Common/IntVector3.h"
#include "Common/Collider.h"

class ResourceManager;
class SceneManager;
class Geometry;
class EffectController;

class ObjectBase
{
public:

	//当たり判定情報
	struct ColParam
	{
		std::unique_ptr<Geometry> geometry_;	//形状情報
		std::shared_ptr<Collider> collider_;	//全体の当たり判定情報
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	ObjectBase(void);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~ObjectBase(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name=""></param>
	virtual void Load(void) = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void) = 0;

	/// <summary>
	/// それぞれの当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定タグ</param>
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol) = 0;

	/// <summary>
	/// モデル情報の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>モデル情報</returns>
	inline const Transform& GetTransform(void) const { return trans_; }

	/// <summary>
	/// サイズの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>サイズ(マップ座標基準)</returns>
	const IntVector3 GetSize(void)const { return size_; }

	/// <summary>
	/// モデルの色を変える
	/// </summary>
	/// <param name="_colorScale">色</param>
	virtual void ChangeModelColor(const COLOR_F _colorScale);

protected:

	// シングルトン参照
	ResourceManager& resMng_;	//リソースマネージャー
	SceneManager& scnMng_;		//シーンマネージャー

	// モデル制御の基本情報
	Transform trans_;

	//大きさ(グリッド基準)
	IntVector3 size_;

	//当たり判定関係
	std::vector<ColParam> colParam_;

	//エフェクト
	std::unique_ptr<EffectController> effect_;

	/// <summary>
	/// 当たり判定作成(形状情報作成後)
	/// </summary>
	/// <param name="_tag">自身の当たり判定タグ</param>
	/// <param name="_Geometry">自身の形状情報</param>
	/// <param name="_notHitTags">衝突させないタグ</param>
	void MakeCollider(const std::vector<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry, const std::vector<Collider::TAG> _notHitTags = {});

	//特定の配列番号の当たり判定の削除
	void DeleteCollider(const int _arrayNum);

	//全当たり判定の削除
	void DeleteAllCollider(void);
};

