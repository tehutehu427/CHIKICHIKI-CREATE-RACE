#pragma once
#include<memory>
#include<vector>
#include"../../Object/Common/Transform.h"
#include"../../Object/Player/Player.h"
#include"../../Object/Player/Process/PlayerInput.h"
#include"../../Object/Common/Capsule.h"
class Player;
class PlayerManager
{
public:
	//**************************************
	//定数
	//**************************************
	static constexpr int PLAYER_NUM = 4;
	//プレイヤー１の座標
	static constexpr float PLAYER_ONE_POS_X = -20.0f;

	//座標の間隔
	static constexpr float DISTANCE_POS = 50.0f;

	//プレイヤー1人
	static constexpr int PLAYER_SINGLE = 1;
	
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
	static void CreateInstance(int _playerNum);

	//静的にインスタンスを取得する
	static PlayerManager& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//プレイヤー同士の当たり判定
	void PlayersCollision(void);

	//カプセル同士の当たり判定(完全ではない)
	bool IsHitCapsules(const std::weak_ptr<Capsule> cap1,const std::weak_ptr<Capsule> cap2);

	/// <summary>
	/// プレイヤー同士の押し出し処理
	/// </summary>
	/// <param name="_pNum1">当たったプレイヤー1人目</param>
	/// <param name="_pNum2">当たったプレイヤー2人目</param>
	void P2PPush(int _pNum1, int _pNum2);
private:


	//静的インスタンス
	static PlayerManager* instance_;

	//*****************************************
	//メンバ変数
	//*****************************************
	//プレイヤー
	std::vector<std::unique_ptr<Player>> players_;

	Player* play_[PLAYER_NUM];


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
	/// コンストラクタ
	/// </summary>
	/// <param name="playerNum">データバンクから人数を持ってくる</param>
	PlayerManager(int _playerNum);
	PlayerManager(const PlayerManager& instance_) = default;
	~PlayerManager(void) = default;
	

};

