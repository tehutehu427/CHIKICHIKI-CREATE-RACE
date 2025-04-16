#pragma once
#include<memory>
#include<vector>
#include"../../Object/Common/Transform.h"
#include"../../Object/Player/Player.h"
class Player;
class PlayerManager
{
public:
	//**************************************
	//定数
	//**************************************
	static constexpr int PLAYER_NUM = 4;
	
	enum class PLAYER_NUM
	{
		PLAYER_ONE,
		PLAYER_TWO,
		PLAYER_THREE,
		PLAYER_FOUR
	};


	//**************************************
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerNum">データバンクから人数を持ってくる</param>
	PlayerManager(int _playerNum);
	~PlayerManager(void)=default;
	//静的にインスタンス生成
	static void CreateInstance(int _playerNum);

	//静的にインスタンスを取得する
	static std::unique_ptr<PlayerManager>& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	//静的インスタンス
	static std::unique_ptr<PlayerManager> instance_;

	//*****************************************
	//メンバ変数
	//*****************************************
	//プレイヤー
	std::vector<std::shared_ptr<Player>> players_;

	//プレイヤー人数
	int playerNum_;

	//*****************************************
	//*****************************************
	//メンバ関数
	//*****************************************
	Transform SetTrans(int _playerNum);
	

};

