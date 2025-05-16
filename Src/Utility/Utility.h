#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Common/IntVector3.h"
#include "../Common/Quaternion.h"
class Utility
{

public:

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	//カラーコード
	static constexpr int BLACK = 0x000000;	//黒
	static constexpr int RED = 0xFF0000;	//赤
	static constexpr int BLUE = 0x0000FF;	//青
	static constexpr int YELLOW = 0xFFFF00;	//黄色
	static constexpr int GREEN = 0x00FF00;	//緑
	static constexpr int CYAN = 0x00FFFF;	//水色
	static constexpr int PINK = 0xFFC0CB;	//桃色
	static constexpr int ORANGE = 0xFFA500;	//オレンジ
	static constexpr int LIME = 0xADFF2F;	//黄緑
	static constexpr int PURPLE = 0x800080;	//紫
	static constexpr int WHITE = 0xFFFFFF;	//白
	static constexpr int BROWN = 0x8B4513;	//茶色

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	//値を反転させる溜めのスケール
	static constexpr float REVERSE_SCALE = -1.0f;

	// 四捨五入
	static int Round(float v);

	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);

	// ラジアン(rad)から度(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);

	// 度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);

	// 0～360度の範囲に収める
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);
	
	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);
	
	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 角度の線形補間
	static double LerpDeg(double start, double end, double t);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// ベジェ曲線
	static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);
	
	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// ベクトルの長さ
	static double Magnitude(const Vector2& v);
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static int SqrMagnitude(const Vector2& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const Vector2& v1, const Vector2& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 球体同士の衝突判定
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	// 球体とカプセルの衝突判定
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, float sphRadius, 
		const VECTOR& capPos1, const VECTOR& capPos2, float capRadius);

	// 比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);

	// 正規化
	static VECTOR Normalize(const Vector2& v);
	static VECTOR VNormalize(const VECTOR& v);

	// 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);

	//待機時間
	static bool IsTimeOver(float& totalTime, const float& waitTime);

	//文字の描画
	//_str : 表示する文字　int : そろえるライン　int : y軸座標　int : 色
	static void DrawStringCenter(std::string _str, int centerLine, int posY, int color);	//中央ぞろえ
	static void DrawStringLeft(std::string _str, int leftLiine, int posY, int color);	//左揃え
	static void DrawStringRight(std::string _str, int rightLine, int posY, int color);	//右揃え

	/// <summary>
	/// とある点からとある点までの移動ベクトルを返す
	/// </summary>
	/// <param name="_start">狙う側</param>
	/// <param name="_goal">向かう先</param>
	/// <param name="_speed">設定速度(未設定だと、方向ベクトルのみを返す)</param>
	/// <returns>向かう先までの移動ベクトル</returns>
	static const VECTOR GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed = 1.0f);

	/// <summary>
	/// 特定の範囲内に座標が含まれてるか調べる
	/// </summary>
	/// <param name="pos"></param>調べたい座標
	/// <param name="leftTop"></param>範囲左上
	/// <param name="rightBotm"></param>範囲右下
	/// <returns></returns>判定結果を返す
	static bool IsPointInRect(const Vector2 pos, const Vector2 leftTop, const Vector2 rightBotm);

	static VECTOR GetWorldPosAtScreen(const Vector2 screenPos, const float distance, const VECTOR cameraPos, const VECTOR cameraDir);

	//符号を返す
	static int GetSign(float f);
	static int GetSign(int f);

	//エクスプローラーからファイルを開く
	static std::string OpenFileDialog();

	//数値の反転
	static VECTOR ReverseValue(const VECTOR _vec);
	static  int ReverseValue(const int _i);
	static float ReverseValue(const float _f);
	static IntVector3 ReverseValue(const IntVector3 _iv);
};

