#pragma once

class DemoPlay
{
public:

	//コンストラクタ
	DemoPlay(void);

	//デストラクタ
	~DemoPlay(void);

	//ロード
	void Load(void);
	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);

private:

	static constexpr float CAMERA_SUB_Z = 100.0f;										//カメラのZ位置を下げる用
	static constexpr VECTOR CAMERA_POS = { 600.0f,1000.0f,-600.0f - CAMERA_SUB_Z };		//カメラ位置
	static constexpr VECTOR CAMERA_TARGET_POS = { 600.0f,-100.0,600.0 - CAMERA_SUB_Z };	//カメラターゲット位置

	float step_;	//カウンタ

	std::vector<VECTOR> revivalPos_;	//復活座標

	//デモステージの生成
	void CreateDemoStage(void);
};

