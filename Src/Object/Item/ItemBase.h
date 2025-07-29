#pragma once
#include "../ObjectBase.h"

class ToonStyle;

class ItemBase : public ObjectBase
{
public:

	//アイテムの種類
	enum class ITEM_TYPE	
	{
		NONE,

		//固定オブジェ
		START,				//スタート
		GOAL,				//ゴール

		//設置オブジェ
		FLOOR,				//床
		MOVE_HORI_FLOOR,	//水平(横)方向移動床
		MOVE_VER_FLOOR,		//垂直(縦)方向移動床
		FENCE,				//柵
		CANNON,				//大砲
		SPIKY,				//とげ
		FAN,				//扇風機

		//FERRIS_WHEEL,		//観覧車
		SLIME_FLOOR,		//スライム床
		SPRING,				//ばね(ジャンプ台)

		//破壊オブジェ
		BOMB_SMALL,			//爆弾(小)
		BOMB_BIG,			//爆弾(大)

		COIN,				//コイン

		MAX,				//最大
	};

	//アイテムの効果
	enum class EFFECT_TYPE
	{
		FIXED,			//固定
		INSTALLATION,	//設置物
		DESTROYER,		//破壊物
	};

	//回転の種類
	enum class ROTATION_TYPE
	{
		HALF_ROTATION,	//半回転
		ONE_ROTATION,	//一回転
	};

	//アイテム数
	static constexpr int ITEM_NUM_MAX = static_cast<int>(ITEM_TYPE::MAX) - 1;	//アイテムの種類数

	//ステータス
	struct Status
	{
		bool isBreak;				//破壊可能か(true:破壊可能)
		bool isGravity;				//重力影響を受けるか(true:受ける)
		ITEM_TYPE itemType;			//アイテムの種類
		EFFECT_TYPE effType;		//アイテムの効果
		ROTATION_TYPE rotType;		//回転の種類
	};

	//コンストラクタ
	ItemBase();
	//デストラクタ
	virtual ~ItemBase()override;

	//アイテムの個別設定
	virtual void SetParam(void) = 0;

	//読み込み
	void Load(void)override;
	//初期化
	virtual void Init(IntVector3 _mapPos ,Quaternion _quaRot, ITEM_TYPE _itemType);
	virtual void Init()override;
	//更新
	virtual void Update(void)override = 0;
	//描画
	virtual void Draw(void)override;

	//全モデルの取得
	const std::vector<int*> GetModels(void)const { return models_; }

	//アイテムのステータス取得
	inline const Status GetStatus(void)const { return status_; }

	//座標の設定
	void SetPos(IntVector3 mapPos);
	//回転の設定
	void SetRotate(Quaternion rot);

	//当たり判定サイズ
	virtual const IntVector3 GetHitSize(void)const { return size_; }
	
	//初期マップ座標の取得
	inline const IntVector3 GetInitMapPos(void)const { return initMapPos_; }	

	//Y回転の取得
	inline const float GetRotY(void)const { return rotY_; }

	//Y回転の設定
	inline void SetRotY(const float _rotY) { rotY_ = _rotY; }

	//移動量の取得
	inline const VECTOR GetMovePow(void)const { return movePow_; }

	//アイテムの値リセット
	virtual void ResetValue(void);

	//マネージャからエフェクトを停止する用
	void EffectAllStop(void);

	/// <summary>
	/// モデルのカラーを設定
	/// </summary>
	/// <param name="_r">赤</param>
	/// <param name="_g">緑</param>
	/// <param name="_b">青</param>
	/// <param name="_a">アルファ値</param>
	virtual void SetModelColor(const float _r, const float _g, const float _b, const float _a);

protected:

	//共通変数
	Status status_;				//ステータス
	IntVector3 initMapPos_;		//初期マップ座標
	float rotY_;				//Y回転
	VECTOR movePow_;			//移動量
	IntVector3 mapSize_;			//マップサイズ
	std::vector<int*> models_;	//全モデル	
	
	//トゥーンスタイル
	std::unique_ptr<ToonStyle> toonStyle_;


	//サイズの倍率調整
	const VECTOR AdjustSizePer(const VECTOR _modelSize)const;	
	
	//シェーダーの設定
	virtual void InitShader();

	/// <summary>
	/// カメラ範囲内か調べる
	/// </summary>
	/// <returns>trueなら範囲内、falseなら範囲外</returns>
	bool IsInCameraView();

private:




};

