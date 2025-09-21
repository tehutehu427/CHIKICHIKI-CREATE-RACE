#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>
class Quaternion
{

public:
	
	//最小の数
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	//回転要素
	double w;	//スカラー値
	double x;	//Xベクトル
	double y;	//Yベクトル
	double z;	//Zベクトル

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Quaternion(void);
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="rad">ラジアン角</param>
	Quaternion(const VECTOR& _rad);
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="w">スカラー値</param>
	/// <param name="x">Xベクトル</param>
	/// <param name="y">Yベクトル</param>
	/// <param name="z">Zベクトル</param>
	Quaternion(double _ww, double _wx, double _wy, double _wz);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Quaternion(void);

	/// <summary>
	/// オイラー角からクォータニオンへ変換
	/// </summary>
	/// <param name="rad">ラジアン角</param>
	/// <returns>オイラー角をもとにしたクォータニオン</returns>
	static Quaternion Euler(const VECTOR& _rad);
	
	/// <summary>
	/// オイラー角からクォータニオンへ変換
	/// </summary>
	/// <param name="radX">Xラジアン角</param>
	/// <param name="radY">Yラジアン角</param>
	/// <param name="radZ">Zラジアン角</param>
	/// <returns>オイラー角をもとにしたクォータニオン</returns>
	static Quaternion Euler(double _radX, double _radY, double _radZ);

	/// <summary>
	/// クォータニオンの合成
	/// </summary>
	/// <param name="q1">クォータニオン1</param>
	/// <param name="q2">クォータニオン2</param>
	/// <returns>q1とq2の合成クォータニオン</returns>
	static Quaternion Mult(const Quaternion& _q1, const Quaternion& _q2);
	
	/// <summary>
	/// クォータニオンの合成
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>合成クォータニオン</returns>
	Quaternion Mult(const Quaternion& _q) const;

	/// <summary>
	/// 指定軸を指定角分、回転させる
	/// </summary>
	/// <param name="rad">ラジアン角</param>
	/// <param name="axis">回転軸</param>
	/// <returns>回転後クォータニオン</returns>
	static Quaternion AngleAxis(double _rad, VECTOR _axis);

	/// <summary>
	/// 座標を回転させる
	/// </summary>
	/// <param name="q">回転用クォータニオン</param>
	/// <param name="pos">回転する座標</param>
	/// <returns>回転後座標</returns>
	static VECTOR PosAxis(const Quaternion& _q, VECTOR _pos);

	/// <summary>
	/// 座標を回転させる
	/// </summary>
	/// <param name="pos">回転する座標</param>
	/// <returns>回転後座標</returns>
	VECTOR PosAxis(VECTOR _pos) const;

	/// <summary>
	/// クォータニオンからオイラー角へ変換
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>クォータニオンを元にしたオイラー角</returns>
	static VECTOR ToEuler(const Quaternion& _q);

	/// <summary>
	/// クォータニオンからオイラー角へ変換
	/// </summary>
	/// <param name=""></param>
	/// <returns>クォータニオンを元にしたオイラー角</returns>
	VECTOR ToEuler(void) const;

	/// <summary>
	/// クォータニオンから行列へ変換
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>クォータニオンを元にした行列</returns>
	static MATRIX ToMatrix(const Quaternion& _q);

	/// <summary>
	/// クォータニオンから行列へ変換
	/// </summary>
	/// <param name=""></param>
	/// <returns>クォータニオンを元にした行列</returns>
	MATRIX ToMatrix(void) const;

	/// <summary>
	/// ベクトルからクォータニオンに変換
	/// </summary>
	/// <param name="dir">方向ベクトル</param>
	/// <returns>ベクトルを元にしたクォータニオン</returns>
	static Quaternion LookRotation(VECTOR _dir);

	/// <summary>
	/// ベクトルからクォータニオンに変換
	/// </summary>
	/// <param name="dir">方向ベクトル</param>
	/// <param name="up">上方向ベクトル</param>
	/// <returns></returns>
	static Quaternion LookRotation(VECTOR _dir, VECTOR _up);

	/// <summary>
	/// 行列からクォータニオンに変換
	/// </summary>
	/// <param name="mat">行列</param>
	/// <returns>行列をもとにしたクォータニオン</returns>
	static Quaternion GetRotation(MATRIX _mat);

	/// <summary>
	/// 前方向ベクトルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>前方向ベクトル</returns>
	VECTOR GetForward(void) const;

	/// <summary>
	/// 後方向ベクトルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>後方向ベクトル</returns>
	VECTOR GetBack(void) const;

	/// <summary>
	/// 右方向ベクトルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>右方向ベクトル</returns>
	VECTOR GetRight(void) const;

	/// <summary>
	/// 左方向ベクトルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>左方向ベクトル</returns>
	VECTOR GetLeft(void) const;

	/// <summary>
	/// 上方向ベクトルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>上方向ベクトル</returns>
	VECTOR GetUp(void) const;

	/// <summary>
	/// 下方向ベクトルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>下方向ベクトル</returns>
	VECTOR GetDown(void) const;

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="q1">クォータニオン1</param>
	/// <param name="q2">クォータニオン2</param>
	/// <returns>q1とq2の内積</returns>
	static double Dot(const Quaternion& _q1, const Quaternion& _q2);

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="b">クォータニオン</param>
	/// <returns>内積</returns>
	double Dot(const Quaternion& _b) const;

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>単位クォータニオン</returns>
	static Quaternion Normalize(const Quaternion& _q);

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name=""></param>
	/// <returns>単位クォータニオン</returns>
	Quaternion Normalized(void) const;

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name=""></param>
	void Normalize(void);

	/// <summary>
	/// 逆クォータニオン
	/// </summary>
	/// <param name=""></param>
	/// <returns>逆クォータニオン</returns>
	Quaternion Inverse(void) const;

	/// <summary>
	/// 球面補間
	/// </summary>
	/// <param name="from">元クォータニオン</param>
	/// <param name="to">回転後クォータニオン</param>
	/// <param name="t">補間比率(0.0～1.0)</param>
	/// <returns>比率に基づいた回転後クォータニオン</returns>
	static Quaternion Slerp(Quaternion _from, Quaternion _to, double _t);

	/// <summary>
	/// ２つのベクトル間の回転量を取得する
	/// </summary>
	/// <param name="fromDir">元ベクトル</param>
	/// <param name="toDir">回転後ベクトル</param>
	/// <returns>回転量クォータニオン</returns>
	static Quaternion FromToRotation(VECTOR _fromDir, VECTOR _toDir);
	
	/// <summary>
	/// ２つのベクトル間の回転量を取得する
	/// </summary>
	/// <param name="from">元クォータニオン</param>
	/// <param name="to">回転後クォータニオン</param>
	/// <param name="maxDegreesDelta">進める角度</param>
	/// <returns>角度に基づいた回転量クォータニオン</returns>
	static Quaternion RotateTowards(const Quaternion& _from, const Quaternion& _to, float _maxDegreesDelta);
	
	/// <summary>
	/// ２つのベクトル間の回転量を取得する
	/// </summary>
	/// <param name="q1">クォータニオン1</param>
	/// <param name="q2">クォータニオン2</param>
	/// <returns>q1とq2の回転角度(デグリー角)</returns>
	static double Angle(const Quaternion& _q1, const Quaternion& _q2);
	
	/// <summary>
	/// 球面補間(範囲制限なし)
	/// </summary>
	/// <param name="a">元クォータニオン</param>
	/// <param name="b">回転後クォータニオン</param>
	/// <param name="t">補間比率</param>
	/// <returns>比率に基づいた回転後クォータニオン</returns>
	static Quaternion SlerpUnclamped(Quaternion _a, Quaternion _b, float _t);
	
	/// <summary>
	/// 単位クォータニオン
	/// </summary>
	/// <param name=""></param>
	/// <returns>単位クォータニオン</returns>
	static Quaternion Identity(void);

	/// <summary>
	/// クォータニオンの長さ
	/// </summary>
	/// <param name=""></param>
	/// <returns>クォータニオンの長さ</returns>
	double Length(void) const;

	/// <summary>
	/// クォータニオンの長さ(二乗)
	/// </summary>
	/// <param name=""></param>
	/// <returns>クォータニオンの長さ(二乗)</returns>
	double LengthSquared(void) const;

	/// <summary>
	/// VECTOR変換
	/// </summary>
	/// <param name=""></param>
	/// <returns>VECTOR(x,y,z)</returns>
	VECTOR xyz(void) const;

	/// <summary>
	/// 対象方向の回転
	/// </summary>
	/// <param name="angle">参照回転</param>
	/// <param name="axis">参照回転軸</param>
	void ToAngleAxis(float* _angle, VECTOR* _axis);

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR _dir) const;

	Quaternion operator*(float& _rhs);
	const Quaternion operator*(const float& _rhs);
	Quaternion operator+(Quaternion& _rhs);
	const Quaternion operator+(const Quaternion& _rhs);
	
};
