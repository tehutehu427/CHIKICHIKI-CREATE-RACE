#pragma once

struct IntVector3
{
public:

	int x;
	int y;
	int z;

	// コンストラクタ
	IntVector3(void);
	// コンストラクタ
	IntVector3(int vX, int vY, int vZ);
	// デストラクタ
	~IntVector3(void);

	//演算
	const IntVector3 operator+(const IntVector3 _value)const;
	void operator+=(const IntVector3 _value);
	const IntVector3 operator-(const IntVector3 _value)const;
	void operator-=(const IntVector3 _value);
	const IntVector3 operator*(const int _value)const;
	void operator*=(const int _value);
	const IntVector3 operator/(const int _value)const;
	void operator/=(const int _value);

private:
};

