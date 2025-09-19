#pragma once

class DemoPlay
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	DemoPlay(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DemoPlay(void);

	/// <summary>
	/// ロード
	/// </summary>
	/// <param name=""></param>
	void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

private:

	//カメラ関係
	static constexpr float CAMERA_SUB_Z = 100.0f;										//カメラのZ位置を下げる用
	static constexpr VECTOR CAMERA_POS = { 600.0f,1000.0f,-600.0f - CAMERA_SUB_Z };		//カメラ位置
	static constexpr VECTOR CAMERA_TARGET_POS = { 600.0f,-100.0,600.0 - CAMERA_SUB_Z };	//カメラターゲット位置

	//カウンタ
	float step_;	

	//復活座標
	std::vector<VECTOR> revivalPos_;	

	//デモステージの生成
	void CreateDemoStage(void);
};

