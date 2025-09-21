#pragma once  
#include <functional>
#include "../Common/Vector2.h"  

class EditController;  

class EditItemReady
{

public:

	enum class READY_PHASE
	{
		NOT_READY,
		PAD_EXPLAIN,	//パッド操作の説明の表示
		CHECK,			//レディするかの確認
		READY,			//レディする
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent">親クラス</param>
	EditItemReady(EditController& _parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EditItemReady(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// 状態を変更する
	/// </summary>
	/// <param name="_ready">変更する状態</param>
	void ChangeReady(const READY_PHASE _ready);

	/// <summary>
	/// 状態を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns>状態取得</returns>
	inline READY_PHASE GetReady(void)const { return ready_; }

private:

	//当たり判定の大きさ
	static constexpr int HIT_WIDTH = 128;	//当たり判定の幅
	static constexpr int HIT_HEIGHT = 64;	//当たり判定の高さ

	//描画関連
	static constexpr int MARGIN = 10;		//描画時の余白
	static constexpr int ALPHA_RATE = 128;	//半透明のアルファ値
	static constexpr int ALPHA_RATE_MAX = 255;	//完全不透明のアルファ値

	//にわとりが出る確率
	static constexpr int CHIKEN_VOICE_RATE = 10;	

	//フェーズ管理
	READY_PHASE ready_;

	//親クラス
	EditController& parent_;
	//当たり判定の大きさ
	Vector2 hitSize_;
	//描画位置
	Vector2 pos_;
	//画像ハンドル
	int  readyImg_;

	//フェーズ管理(遷移時の初期処理)
	std::map<READY_PHASE, std::function<void(void)>> phaseChanges_;

	//フェーズごとの処理
	std::function<void(void)> phaseUpdate_;	//更新処理
	std::function<void(void)> phaseDraw_;	//描画処理

	//フェーズ遷移時の処理
	void ChangePhaseNotReady(void);
	void ChangePhasePadExplain(void);
	void ChangePhaseCheck(void);
	void ChangePhaseReady(void);

	//フェーズごとの更新処理
	void UpdateNotReady(void);
	void UpdatePadExplain(void);
	void UpdateCheck(void);
	void UpdateReady(void);

	//フェーズごとの描画処理
	void DrawNotReady(void)const;
	void DrawPadExplain(void);
	void DrawCheck(void);
	void DrawReady(void);

};
