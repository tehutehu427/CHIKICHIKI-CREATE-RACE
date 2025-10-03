#pragma once
#include"../../Object/Player/Player.h"		//これ消したい

class Player;

class PlayerManager
{

public:

	//プレイヤー最大人数
	static constexpr int PLAYER_NUM_MAX = 4;
	//プレイヤー1人
	static constexpr int PLAYER_SINGLE = 1;
	//プレイヤーカラー
	static constexpr COLOR_F PLAYER_COLOR[PLAYER_NUM_MAX] =
	{		
		{0.6f,0.8f,1.0f,1.0f},
		{1.0f,0.6f,0.9f,1.0f},
		{0.7f,1.0f,0.8f,1.0f},
		{1.0f,1.0f,0.7f,1.0f},
	};

	//プレイヤーナンバー
	enum class PLAYER
	{
		PLAYER_ONE,
		PLAYER_TWO,
		PLAYER_THREE,
		PLAYER_FOUR
	};
	

	/// <summary>
	/// 静的にインスタンス生成
	/// </summary>
	/// <param name="_playerNum">プレイヤー人数</param>
	static void CreateInstance(void);

	/// <summary>
	/// 解放
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// 静的にインスタンスを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static PlayerManager& GetInstance(void);

	/// <summary>
	/// ロード
	/// </summary>
	/// <param name=""></param>
	void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// モデル情報の取得
	/// </summary>
	/// <param name="_num">プレイヤー番号</param>
	/// <returns>モデル情報の取得</returns>
	const Transform& GetPlayerTransform(const int _num);

	/// <summary>
	/// プレイヤー情報の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤー情報</returns>
	std::vector<std::unique_ptr<Player>>& GetPlayers(void);

	/// <summary>
	///プレイヤーコイン枚数
	/// </summary>
	/// <param name="_num">プレイヤー番号</param>
	/// <returns>コイン枚数</returns>
	const int GetPlayerCoinNum(const int _num);

	/// <summary>
	/// ゴールタイムの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>ゴールタイム</returns>
	const std::vector<float> GetGoalTime(void);

	/// <summary>
	/// 初期座標に戻す
	/// </summary>
	/// <param name="_worldPos">戻したい座標</param>
	void SetInitPos(VECTOR _worldPos);

	/// <summary>
	/// 指定したプレイヤーがゴール済みか調べる
	/// </summary>
	/// <param name="_playerIndex">プレイヤーインデックス</param>
	/// <returns>ゴールしてたらtrue,してなければfalse</returns>
	const bool IsPlayerGoal(const int _playerIndex);
	
	/// <summary>
	/// 指定したプレイヤーが倒れたか調べる
	/// </summary>
	/// <param name="_playerIndex">プレイヤーインデックス</param>
	/// <returns>倒れてたらtrue,なければfalse</returns>
	const bool IsPlayerDeath(const int _playerIndex);

	/// <summary>
	///　全てのプレイヤーが操作を終えているか
	/// </summary>
	/// <returns>終えてたらtrue,なければfalse</returns>
	bool IsPlayersEnd(void);

	/// <summary>
	/// 各プレイヤーごとのUIを描画
	/// </summary>
	/// <param name="_playerIndex">プレイヤーインデックス</param>
	void DrawUI(const int _playerIndex);

	/// <summary>
	/// 複数プレイヤーのステータスアップ
	/// </summary>
	/// <param name=""></param>
	void BuffPlayer(void);

private:

	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };
	//ハート画像サイズ
	static constexpr float HEART_IMG_SCL = 0.05f;				//画像の大きさ
	static constexpr int HEART_IMG_SIZE_X = static_cast<int>(640 * HEART_IMG_SCL);//Xの画像サイズ
	static constexpr int HEART_IMG_SIZE_Y = static_cast<int>(640 * HEART_IMG_SCL);//Yの画像サイズ

	//初期座標
	static constexpr float START_POS = 50.0f;
	//静的インスタンス
	static PlayerManager* instance_;
	//ゴール時間
	std::vector<float>goalTime_;

	//プレイヤー
	std::vector<std::unique_ptr<Player>> players_;
	//プレイヤー人数
	int playerNum_;
	//リスポーンハートの画像番号
	int imgRespawnHeart_; 

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	PlayerManager(void);
	PlayerManager(const PlayerManager& instance_) = default;
	~PlayerManager(void);

	//プレイヤー番号ごとでモデル情報を決定する
	Transform FixTrans(int _playerNum);

	//プレイヤーカラーを設定
	void InitPlayerColor();
};

