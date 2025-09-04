#pragma once
#include <unordered_map>
#include <functional>

class EventManager
{

public:

	/// <summary>
	/// イベント種類
	/// </summary>
	enum class EVENT_TYPE
	{
		NONE,		//特になし
		STATE_UP,	//強化
		RESPAWN,	//一度だけ復活
		WIGGLE,		//画面をくねくねさせる
		FISH_EYE,	//魚眼
		THREE_POINT,//ポイントアップ
		SAND_STORM,	//砂嵐
		MAX
	};

	/// <summary>
	/// 静的にインスタンスを生成
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static EventManager& GetInstance(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// イベント種類を返す
	/// </summary>
	/// <returns>イベント種類</returns>
	const EVENT_TYPE GetEventType()const { return eventType_; }

	/// <summary>
	/// イベントの設定
	/// </summary>
	/// <param name="type"></param>
	void SetEventType(const EVENT_TYPE _type);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:

	//イベント状態
	EVENT_TYPE eventType_;

	//静的インスタンス
	static EventManager* instance_;

	//イベント別設定処理
	std::unordered_map<EVENT_TYPE, std::function<void(void)>> eventFunc_;

	//イベント別設定処理登録
	void RegisterSet(const EVENT_TYPE _type, std::function<void(void)> _func);

	//イベント種類別初期設定
	void SetNone(void) {}
	void SetRespawn(void);
	void SetThreePoint(void);
	void SetStateUp(void);
	void SetSandstorm(void);
	void SetWiggle(void);
	void SetFishEye(void);

	//コンストラクタ
	EventManager(void);

	//コピーコンストラクタ及び代入演算の禁止
	EventManager(const EventManager& instance_) = delete;
	void operator= (const EventManager& instance_) = delete;

	//デストラクタ
	~EventManager(void);

};

