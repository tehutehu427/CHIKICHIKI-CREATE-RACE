#pragma once
#include<memory>
#include<vector>
#include"../../Object/Common/Transform.h"
#include"../../Object/Player/Player.h"
#include"../../Object/Player/Process/PlayerInput.h"
//#include"../../Object/Common/Capsule.h"
class Player;
class PlayerManager
{
public:
	//**************************************
	//定数
	//**************************************
	static constexpr int PLAYER_NUM = 4;

	//プレイヤー1人
	static constexpr int PLAYER_SINGLE = 1;

	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };

	//初期座標
	static constexpr float START_POS = 50.0f;

	static constexpr Collider::TAG PLAYER_1_TAG = Collider::TAG::PLAYER1;
	static constexpr Collider::TAG PLAYER_2_TAG = Collider::TAG::PLAYER2;
	static constexpr Collider::TAG PLAYER_3_TAG = Collider::TAG::PLAYER3;
	static constexpr Collider::TAG PLAYER_4_TAG = Collider::TAG::PLAYER4;

	
	enum class PLAYER
	{
		PLAYER_ONE,
		PLAYER_TWO,
		PLAYER_THREE,
		PLAYER_FOUR
	};


	//**************************************
	

	/// <summary>
	/// 静的にインスタンス生成
	/// </summary>
	/// <param name="_playerNum">プレイヤー人数</param>
	static void CreateInstance(void);

	//解放
	void Destroy(void);

	//静的にインスタンスを取得する
	static PlayerManager& GetInstance(void);
	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);

	//プレイヤー同士の当たり判定
	void PlayersCollision(void);

	//カプセル同士の当たり判定(完全ではない)
	//bool IsHitCapsules(const std::weak_ptr<Capsule> cap1,const std::weak_ptr<Capsule> cap2);


	//*****************************************
	//ゲッタ
	//*****************************************
	//モデル情報ゲッタ
	const Transform& GetPlayerTransform(const int _num) { return players_[_num]->GetTransform(); }

	//移動後座標
	const VECTOR GetPlayerMovedPos(const int _num) { return players_[_num]->GetMovedPos(); }

	const std::vector<bool>GetPlayersIsDeath(void);

	std::vector<std::unique_ptr<Player>>&GetPlayers(void) { return players_; }

	Player& GetPlayer(int _num) { return *players_[_num]; }

	//****************************************
	//セッタ
	//****************************************
	//初期座標に戻す
	void SetInitPos(VECTOR _worldPos);

	/// <summary>
	///プレイヤー全員がゴールに行ったかどうかを判定
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:全員ゴールに行った　false:誰か一人でもゴールに行ってない</returns>
	std::vector<bool> IsGoalPlayers(void);

	//当たり判定で調べる座標
	IntVector3 GetPlayerColPos(const int _num) { return players_[_num]->GetColPos(); }

	//全員がゴールしてるか死んでるか
	bool IsPlayersEnd(void);



private:


	//静的インスタンス
	static PlayerManager* instance_;

	//*****************************************
	//メンバ変数
	//*****************************************
	//プレイヤー
	std::vector<std::unique_ptr<Player>> players_;

	//プレイヤーゴール判定
	std::vector<bool>isGoal_;

	//プレイヤー生存判定
	std::vector<bool>isDeath_;


	//プレイヤー人数
	int playerNum_;

	//コントロール関数
	PlayerInput::CNTL cntl_;

	//*****************************************
	//*****************************************
	//メンバ関数
	//*****************************************

	//プレイヤー番号ごとでモデル情報を決定する
	Transform FixTrans(int _playerNum);

	/// <summary>
	/// パンチの当たり判定
	/// </summary>
	/// <param name="p1">判定したい1人目のプレイヤ</param>
	/// <param name="p2">判定したい2人目のプレイヤ</param>
	void PunchPlayersColl(int p1,int p2);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerNum">データバンクから人数を持ってくる</param>
	PlayerManager(void);
	PlayerManager(const PlayerManager& instance_) = default;
	~PlayerManager(void);
	

};

