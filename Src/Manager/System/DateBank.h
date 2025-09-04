#pragma once
#include <string>
#include "../Object/Item/ItemBase.h"

class DateBank
{
public:

	/// <summary>
	/// 操作種類
	/// </summary>
	enum class TYPE
	{
		KEY_BORD,	//キーボード操作		ソロ用
		CONTROLLER,	//コントローラー操作	マルチ用
		ERORE		//プレイヤー人数がerore
	};	
	
	//通常のスコア量
	static constexpr  int DEFAULT_SCORE = 8;

	//通常のラウンド制限数
	static constexpr int ROUND_LIMIT = 5;


	/// <summary>
	/// 明示的にインステンスを生成する
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// 静的インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンスを返す</returns>
	static DateBank& GetInstance(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// プレイヤー人数を設定
	/// </summary>
	/// <param name="num">プレイヤー人数</param>
	/// <returns>正しく設定できればtrue,問題が起きればfalse</returns>
	bool SetPlayerNum(int _num);
	
	/// <summary>
	/// マルチでのクリアスコアの設定
	/// </summary>
	/// <param name="_clearScore"></param>
	void SetMultiClearScore(const int _clearScore) { multiClearScore_ = _clearScore; }

	/// <summary>
	/// プレイヤー人数を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤー人数</returns>
	int GetPlayerNum(void);

	/// <summary>
	/// 操作種類を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>操作種類</returns>
	TYPE GetType(void);

	/// <summary>
	/// ステージの番号を設定
	/// </summary>
	/// <param name="_stageNo">ステージ番号</param>
	/// <returns></returns>
	const void SetStageNo(const int _stageNo) { stageNo_ = _stageNo; }

	/// <summary>
	/// ステージの番号を返す
	/// </summary>
	/// <returns>ステージ番号</returns>
	const int GetStageNo()const { return stageNo_; }

	/// <summary>
	/// マルチのクリア条件スコアを返す
	/// </summary>
	/// <returns>マルチのクリア条件スコア</returns>
	const int GetMultiClearScore() const { return multiClearScore_; }

	/// <summary>
	/// アイテムの名前を返す
	/// </summary>
	/// <param name="_type">アイテムの種類</param>
	/// <returns>アイテムネーム</returns>
	const std::string GetItemName(const ItemBase::ITEM_TYPE _type)const { return itemNames_[static_cast<int>(_type)]; }

	/// <summary>
	/// アイテムの配置スキップの有無を返す
	/// </summary>
	/// <returns>trueの場合あり、falseの場合なし</returns>
	const bool IsItemSetSkip() const { return isItemSetSkip_; }

	/// <summary>
	/// アイテムの配置スキップの有無を設定
	/// </summary>
	/// <param name="_isSkip">スキップの有無</param>
	void SetItemSetSkip(const bool _isSkip) { isItemSetSkip_ = _isSkip; }

	/// <summary>
	/// ラウンド制限を返す
	/// </summary>
	/// <returns>ラウンド制限</returns>
	const int GetRoundLimit()const { return roundLimit_; }

	/// <summary>
	/// ラウンド制限を設定
	/// </summary>
	/// <param name="_round">ラウンド数</param>
	void SetRoundLimit(const int _round) { roundLimit_ = _round; }

	/// <summary>
	/// フルスクリーンにするか返す
	/// </summary>
	/// <returns>フルスクリーン判定</returns>
	const bool IsFullScreen()const { return isFullScreen_; }

	/// <summary>
	/// フルスクリーンの設定
	/// </summary>
	/// <param name="_isFullScreen"></param>
	void SetFullScreen(const bool _isFullScreen);

private:


	static DateBank* instance_;
	DateBank(void);
	DateBank(const DateBank& instance_) = default;
	~DateBank(void);
	int playerNum_;			//プレイヤーの人数
	int maxPlayerNum_;		//コントローラー数よりプレイヤー人数が多くならないようにする
	int stageNo_;			//ステージ番号
	int multiClearScore_;	//マルチでのクリアとなるスコア量
	int roundLimit_;		//ラウンド制限
	bool isItemSetSkip_;	//アイテムセットスキップの有無
	bool isFullScreen_;		//フルスクリーンの判定

	//種類ごとの名前
	std::string itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::MAX)];
};

