#pragma once
#include<memory>
#include<vector>
#include"../../Object/Common/Transform.h"
#include"../../Object/Player/Player.h"
#include"../../Object/Player/Process/PlayerInput.h"
class Player;
class PlayerManager
{
public:
	//**************************************
	//定数
	//**************************************

	//プレイヤー１の座標
	static constexpr float PLAYER_ONE_POS_X = -20.0f;

	//座標の間隔
	static constexpr float DISTANCE_POS = 20.0f;

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
private:


	//静的インスタンス
	static PlayerManager* instance_;

	//*****************************************
	//メンバ変数
	//*****************************************
	//プレイヤー
	std::vector<std::shared_ptr<Player>> players_;

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

