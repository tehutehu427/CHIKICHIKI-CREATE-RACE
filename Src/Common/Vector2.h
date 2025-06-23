#pragma once
class Vector2
{
public:

	int x;
	int y;

	// コンストラクタ
	Vector2(void);

	// コンストラクタ
	Vector2(int vX, int vY);

	// デストラクタ
	~Vector2(void);

	/// <summary>
	/// Vector2同士の足し算
	/// </summary>
	/// <param name="_value1">加算する値</param>
	/// <param name="_value2">加算する値</param>
	/// <returns>加算した値</returns>
	static Vector2 AddVector2(const Vector2& _value1, const Vector2& _value2);

	/// <summary>
	/// Vector2同士の引き算
	/// </summary>
	/// <param name="_value1">引かれる値</param>
	/// <param name="_value2">引く値</param>
	/// <returns>減算した値</returns>
	static Vector2 SubVector2(const Vector2& _value1, const Vector2& _value2);

};

