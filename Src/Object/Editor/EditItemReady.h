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

	EditItemReady(EditController& parent);
	~EditItemReady();

	void Init();
	void Update();
	void Draw();

	void ChangeReady(const READY_PHASE _ready);
	inline READY_PHASE GetReady(void)const { return ready_; }
private:

	static constexpr int HIT_WIDTH = 128;	//当たり判定の幅
	static constexpr int HIT_HEIGHT = 64;	//当たり判定の高さ
	static constexpr int MARGIN = 10;		//描画時の余白
	static constexpr int CHIKEN_VOICE_RATE = 10;	//にわとりが出る確率
	static constexpr int ALPHA_RATE = 128;	//半透明のアルファ値
	static constexpr int ALPHA_RATE_MAX = 255;	//完全不透明のアルファ値
	//フェーズ管理
	READY_PHASE ready_;

	//フェーズ管理(遷移時の初期処理)
	std::map<READY_PHASE, std::function<void(void)>> phaseChanges_;

	//フェーズごとの処理
	std::function<void(void)> phaseUpdate_;
	std::function<void(void)> phaseDraw_;

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

	EditController& parent_;
	Vector2 hitSize_;
	Vector2 pos_;
	int  readyImg_;
};
