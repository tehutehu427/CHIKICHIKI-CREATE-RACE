#pragma once
#include <memory>
#include <chrono>
#include "../Common/Fader.h"

// 推奨しませんが、どうしても使いたい方は
#define mainCamera SceneManager::GetInstance().GetCamera().lock()

class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// 先頭の（Updataが呼ばれる）シーンを切り替える
	/// </summary>
	/// <param name="scene">切り替え先のシーン</param>
	void CreateScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// すべてのシーンを切り替える
	/// </summary>
	/// <param name="scene">切り替え先のシーン</param>
	void ChangeAllScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdataしか呼ばれません。
	/// </summary>
	/// <param name="scene">積むシーン</param>
	void PushScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// スタックの頭のシーンを削除する。
	/// ただし、スタック上にシーンが一つしかない場合は、削除しない。
	/// </summary>
	void PopScene();

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);
	
	// フェード開始
	inline void StartFadeIn(void) { fader_->SetFade(Fader::STATE::FADE_IN); }

	// シーンIDの取得
	inline SCENE_ID GetSceneID(void) const { return sceneId_; }

	// デルタタイムの取得
	inline float GetDeltaTime(void) const { return deltaTime_; }

	// 経過時間を返す
	inline float GetTotalTime(void) const { return totalTime_; }

	// カメラの取得
	std::weak_ptr<Camera> GetCamera(void) const { return camera_; }

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// フェード
	std::unique_ptr<SceneBase> scene_;

	// 各種シーン
	std::unique_ptr<Fader> fader_;

	// カメラ
	std::shared_ptr<Camera> camera_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	//経過時間
	float totalTime_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager& manager) = default;
	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

};
