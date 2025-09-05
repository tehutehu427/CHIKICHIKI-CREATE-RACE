#pragma once
#include <memory>
#include <unordered_map>

class PostEffectBase;

class PostEffectManager
{
public:

	//種類
	enum class TYPE
	{
		NONE,
		WIGGLE,
		FISH_EYE,
		SANDSTORM,
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
	static PostEffectManager& GetInstance(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// エフェクトを変更
	/// </summary>
	/// <param name="_type"></param>
	void ChangeEffectType(const TYPE _type) { type_ = _type; }

private:

	//静的インスタンス
	static PostEffectManager* instance_;

	//エフェクトスクリーン
	int postEffectScreen_;

	//種類
	TYPE type_;

	//ポストエフェクト管理用マップ
	std::unordered_map<TYPE, std::unique_ptr<PostEffectBase>> postMap_;

};

