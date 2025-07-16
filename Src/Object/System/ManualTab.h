#pragma once
#include <unordered_map>
#include "../../Common/Vector2.h"
#include "../../Scene/SceneBase.h"
#include "../../Manager/System/SceneManager.h"

class KeyConfig;

class ManualTab
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		WAIT,
		DISPLAY,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_padCursolPos">パッドコントローラーの座標</param>
	ManualTab(const Vector2& _padCursolPos = {});

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ManualTab();
	
	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// マニュアルが表示中か返す
	/// </summary>
	/// <returns>表示中ならtrue,そうでなければfalse</returns>
	inline const bool IsDisplay()const { return state_ == STATE::DISPLAY; }

private:

	//アイコンサイズ
	static constexpr int ICON_SIZE = 128;
	
	//半径
	static constexpr float RADIUS = 64.0f;

	//三角形数
	static constexpr int TRIANGLE_NUM = 2;

	//三角形サイズ
	static constexpr int TRIANGLE_SIZE = 128;

	//パッドのカーソル位置
	const  Vector2& padCursorPos_;

	Vector2 trianglePos_[TRIANGLE_NUM];

	//入力管理
	KeyConfig& key_;

	//アイコン画像
	int imgIcon_;

	//画像用インデックス
	int index_;

	//三角形画像
	int imgTriangle_;

	//マニュアル画像
	std::vector<int> imgs_;

	//状態
	STATE state_;

	//UIの座標位置
	Vector2 uiPos_;

	//シーンID
	SceneManager::SCENE_ID sceneId_;

	//状態別処理
	std::unordered_map<STATE, SceneBase::ProcessFunction> stateFunc_;

	//シーン別に異なる処理を行う
	std::unordered_map<SceneManager::SCENE_ID, std::function<void()>> updateFunc_;
	std::unordered_map<SceneManager::SCENE_ID, std::function<void()>> loadFunc_;

	//処理の登録
	void RegisterStateFunction(const STATE _state, SceneBase::ProcessFunction _func);
	void RegisterUpdateBySceneFunction(const SceneManager::SCENE_ID _scene, std::function<void()> _func);
	void RegisterLoadBySceneFunction(const SceneManager::SCENE_ID _scene, std::function<void()>_func);

	//状態遷移
	void ChangeState(const STATE _state) { state_ = _state; }

	//更新関数
	void UpdateWait();
	void UpdateDisplay();

	//描画関数
	void DrawWait();
	void DrawDisplay();

	//シーン別にリソースを読み込み
	void LoadSelectResource();
	void LoadFreeResource();

	//シーン別に更新処理
	void UpdateSelectScene();
	void UpdateFreeMode();

	//クリックしたか調べる
	bool IsClickTriangle(const int _index);

};