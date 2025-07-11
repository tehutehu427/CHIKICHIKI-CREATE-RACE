#pragma once
#include <memory>
#include <chrono>
#include <vector>
#include "../../Common/Fader.h"

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
		SELECT,
		GAME,
		SOLO,
		MULTI,
		FREE,
		MAX
	};
	
	/// <summary>
	/// インスタンスを生成
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンスを返す</returns>
	static SceneManager& GetInstance(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 3D関連の初期化
	/// </summary>
	/// <param name=""></param>
	void Init3D(void);

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

	/// <summary>
	/// リソースの破棄
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// シーンの状態遷移
	/// </summary>
	/// <param name="nextId">遷移先のシーン</param>
	void ChangeScene(SCENE_ID nextId);
	
	/// <summary>
	/// フェード開始
	/// </summary>
	/// <param name=""></param>
	void StartFadeIn(void);

	/// <summary>
	/// 分割描画の設定
	/// </summary>
	/// <param name="_isSplitMode">分割する場合true、しない場合false</param>
	void SetIsSplitMode(const bool _isSplitMode) { isSplitMode_ = _isSplitMode; }

	/// <summary>
	/// シーンIDを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>シーンID</returns>
	SCENE_ID GetSceneID(void) const { return sceneId_; }

	/// <summary>
	/// メインスクリーンを返す
	/// </summary>
	/// <returns>メインスクリーン</returns>
	int GetMainScreen() const { return mainScreen_; }
	/// <summary>
	/// スクリーンを返す
	/// </summary>
	/// <returns>メインスクリーン</returns>
	int GetScreen(int _screenNum) const { return splitScreens_[_screenNum]; }

	/// <summary>
	/// デルタタイムの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>デルタタイム</returns>
	float GetDeltaTime(void) const { return deltaTime_; }

	/// <summary>
	/// 経過時間を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>経過時間</returns>
	float GetTotalTime(void) const { return totalTime_; }

	/// <summary>
	/// カメラを取得
	/// </summary>
	/// <param name="_playerIndex">プレイヤー番号</param>
	/// <returns>指定したプレイヤー番号のカメラ</returns>
	std::weak_ptr<Camera> GetCamera(const int _playerIndex) const;

	int GetScreenIndex(void)const { return screenIndex_; }

private:

	//条件人数
	static constexpr int CASE_VALUE = 3;

	// 静的インスタンス
	static SceneManager* instance_;

	//シーンID
	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// 各種シーン	
	std::unique_ptr<SceneBase> scene_;

	// フェード
	std::unique_ptr<Fader> fader_;

	// カメラ
	std::vector<std::shared_ptr<Camera>> cameras_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	//経過時間
	float totalTime_;

	//メインスクリーン
	int mainScreen_;	
	
	//画面分割を行うか
	bool isSplitMode_;

	//分割スクリーンのインデックス
	int screenIndex_ ;

	//分割用スクリーン
	std::vector<int> splitScreens_;

	//カメラ生成
	void CreateCameras(const int _playerNum);

	//分割スクリーンの生成
	void CreateSplitScreen(const int _playerNum);
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager& manager) = default;
	// デストラクタも同様
	~SceneManager(void);

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

	//マルチ画面の描画
	void DrawMultiScreen();

};
