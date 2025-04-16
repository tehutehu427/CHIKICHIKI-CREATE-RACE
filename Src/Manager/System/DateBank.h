#pragma once
class DateBank
{
public:
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static DateBank& GetInstance(void);

	void Init(void);

	bool SetPlayerNum(int num);

	int GetPlayerNum(void);
protected:

private:
	static DateBank* instance_;
	DateBank(void);
	DateBank(const DateBank& instance_) = default;
	~DateBank(void) = default;
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする
};

