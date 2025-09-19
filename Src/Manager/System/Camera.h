#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "KeyConfig.h"

class Transform;

class Camera
{

public:

	// カメラスピード(度)
	static constexpr float SPEED = 1.0f;		//カメラのスピード倍率
	static constexpr float SPEED_PAD = 0.0015f;	//カメラのスピードパッド時
	static constexpr float SPEED_MOUSE = 0.05f;	//カメラのスピードマウス時

	//カメラズームスピード
	static constexpr float ZOOM_SPEED = 5.0f;

	//カメラのズーム範囲
	static constexpr float ZOOM_RADIUS = 300.0f;
	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 0.0f, -400.0f };
	//static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, -1000.0f, -200.0f };

	//FPSの時の相対座標
	static constexpr VECTOR FPS_LOCAL_F2C_POS = { 0.0f, 130.0f, 20.0f };

	//FPS用
	static constexpr VECTOR FPS_LOCAL_F2T_POS = { 0.0f, 0.0f, 2000.0f };

	//固定上用
	static constexpr VECTOR FIXED_LOCAL_P2T_POS = { 0.0f, -1500.0f, 0.0f };

	//斜め上用
	static constexpr VECTOR FIXED_DIAGONAL_TARGET_POS = { 2000.0f, 00.0f, 3000.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 100.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 60.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	//FPSの上限角
	static constexpr float FPS_LIMIT_X_UP_RAD = -80.0f * (DX_PI_F / 180.0f);
	static constexpr float FPS_LIMIT_X_DW_RAD = 70.0f * (DX_PI_F / 180.0f);

	
	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		FOLLOW_ROTATION,	//ついじゅうしながら自動回転操作不能
		SELF_SHOT,
		//FPS,
		FREE_CONTROLL,
		FIXED_UP,
		FIXED_DIAGONAL,	//斜め固定
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	Camera(int _playerNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Camera(void);

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
	/// 描画前処理
	/// </summary>
	/// <param name=""></param>
	void SetBeforeDraw(void);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// カメラ位置の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラ座標</returns>
	VECTOR GetPos(void) const;
	/// <summary>
	/// カメラの操作角度の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>角度</returns>
	VECTOR GetAngles(void) const;
	/// <summary>
	/// カメラの注視点を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラの注視点</returns>
	VECTOR GetTargetPos(void) const;

	/// <summary>
	/// カメラの角度を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラ角度</returns>
	Quaternion GetQuaRot(void) const;
	/// <summary>
	/// X回転を抜いたカメラ角度
	/// </summary>
	/// <param name=""></param>
	/// <returns>X回転を抜いたカメラ角度</returns>
	Quaternion GetQuaRotOutX(void) const;
	/// <summary>
	///  カメラの前方方向
	/// </summary>
	/// <param name=""></param>
	/// <returns>前方方向</returns>
	VECTOR GetForward(void) const;

	/// <summary>
	///  カメラの設定
	/// </summary>
	/// <param name=""></param>
	void CameraSetting(void);

	/// <summary>
	/// カメラモードの変更
	/// </summary>
	/// <param name="_mode">変更するモード</param>
	void ChangeMode(MODE _mode);

	/// <summary>
	/// カメラのモードを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラのモード</returns>
	MODE GetMode(void) const { return mode_; }

	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="_follow">追従対象</param>
	void SetFollow(const Transform* _follow);

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="_pos">座標</param>
	void SetPos(VECTOR _pos) { pos_ = _pos; }
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="_angles">回転</param>
	void SetAngles(VECTOR _angles) { angles_ = _angles; }
	/// <summary>
	/// 注視点を設定
	/// </summary>
	/// <param name="_pos"></param>
	void SetTargetPos(VECTOR _pos) { targetPos_ = _pos; }
private:

	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	// カメラのローカル座標
	VECTOR localPos_;

	//PADの番号
	KeyConfig::JOYPAD_NO padNo_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_= {0.0f,0.0f,0.0f};

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラ角度
	Quaternion rot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	//FPS用の位置同期
	void SyncFollowFPS(void);

	// カメラ操作
	void ProcessRot(void);
	void ProcessZoom(void);
	//マウスでのカメラ操作
	//void ProcessRotMause(float* x_m, float* y_m, const float fov_per = 1.0f);
	// モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowRotation(void);
	void SetBeforeDrawSelfShot(void);
	//void SetBeforeDrawFPS(void);
	void SetBeforeDrawFreeControll(void);
	void SetBeforeDrawFixedUp(void);
	void SetBeforeDrawFixedDiagonal(void);
};

