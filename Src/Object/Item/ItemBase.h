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

	//ステータス
	struct Status
	{
		bool isBreak;				//破壊可能か(true:破壊可能)
		bool isGravity;				//重力影響を受けるか(true:受ける)
		ITEM_TYPE itemType;			//アイテムの種類
		EFFECT_TYPE effType;		//アイテムの効果
		ROTATION_TYPE rotType;		//回転の種類
	};

	//アイテム数
	static constexpr int ITEM_NUM_MAX = static_cast<int>(ITEM_TYPE::MAX) - 1;	//アイテムの種類数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	ItemBase(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~ItemBase(void)override;

	/// <summary>
	/// アイテムの個別設定
	/// </summary>
	/// <param name=""></param>
	virtual void SetParam(void) = 0;

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name=""></param>
	void Load(void)override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_mapPos">座標(マップサイズ基準)</param>
	/// <param name="_quaRot">回転</param>
	/// <param name="_itemType">アイテムの種類</param>
	virtual void Init(IntVector3 _mapPos ,Quaternion _quaRot, ITEM_TYPE _itemType);
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void)override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void)override = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void)override;

	/// <summary>
	/// 全モデルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>モデル配列</returns>
	const std::vector<int*> GetModels(void)const { return models_; }

	/// <summary>
	/// アイテムのステータス取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>アイテムのステータス</returns>
	inline const Status GetStatus(void)const { return status_; }

	/// <summary>
	/// 当たり判定サイズ
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たり判定サイズ(マップ座標基準)</returns>
	virtual const IntVector3 GetHitSize(void)const { return size_; }

	/// <summary>
	/// 初期マップ座標の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>初期マップ座標</returns>
	inline const IntVector3 GetInitMapPos(void)const { return initMapPos_; }

	/// <summary>
	/// Y回転の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>Y回転</returns>
	inline const float GetRotY(void)const { return rotY_; }

	/// <summary>
	/// 移動量の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動量</returns>
	inline const VECTOR GetMovePow(void)const { return movePow_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="mapPos">座標(マップ座標基準)</param>
	void SetPos(IntVector3 mapPos);

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotate(Quaternion rot);

	/// <summary>
	/// Y回転の設定
	/// </summary>
	/// <param name="_rotY">Y回転</param>
	inline void SetRotY(const float _rotY) { rotY_ = _rotY; }

	/// <summary>
	/// アイテムの値リセット
	/// </summary>
	/// <param name=""></param>
	virtual void ResetValue(void);

	/// <summary>
	/// マネージャからエフェクトを停止する用
	/// </summary>
	/// <param name=""></param>
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
	IntVector3 mapSize_;		//マップサイズ
	std::vector<int*> models_;	//全モデル	
	
	//トゥーンスタイル
	std::unique_ptr<ToonStyle> toonStyle_;

	/// <summary>
	/// サイズの倍率調整
	/// </summary>
	/// <param name="_modelSize">サイズ倍率</param>
	/// <returns></returns>
	const VECTOR AdjustSizePer(const VECTOR _modelSize)const;	
	
	/// <summary>
	/// シェーダーの設定
	/// </summary>
	/// <param name=""></param>
	virtual void InitShader(void);

	/// <summary>
	/// カメラ範囲内か調べる
	/// </summary>
	/// <returns>trueなら範囲内、falseなら範囲外</returns>
	const bool IsInCameraView(void);
};

