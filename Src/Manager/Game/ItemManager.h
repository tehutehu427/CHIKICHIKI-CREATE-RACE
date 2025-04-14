#pragma once
class ItemManager
{
public:
	void Init(void);

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ItemManager& GetInstance(void);
protected:

private:
	static ItemManager* instance_;

	ItemManager(void);
	ItemManager(const ItemManager& instance_) = default;
	~ItemManager(void) = default;
};

