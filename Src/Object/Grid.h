#pragma once

class Grid
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Grid(void);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	~Grid(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);
	/// <summary>
	/// 解放処理
	/// </summary>
	/// <param name=""></param>
	void Release(void);

};