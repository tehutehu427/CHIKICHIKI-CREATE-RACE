#pragma once
#include <functional>
#include <unordered_map>
#include "../../Common/Vector2.h"
#include "../../Scene/Game/GameScene.h"

class GameScene;

class CheckChangePhase
{
public:

	//アイコンサイズ
	static constexpr int ICON_SIZE_X = 160;
	static constexpr int ICON_SIZE_Y = 80;

	/// <summary>
	/// アイコンの種類
	/// </summary>
	enum class ICON_TYPE
	{
		PLAY,
		EDIT,
		MAX
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CheckChangePhase(const Vector2& _padCursorPos = {});

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CheckChangePhase(void) = default;

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_parent">親クラス</param>
	void Update(GameScene& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 次のフェーズ遷移を設定
	/// </summary>
	/// <param name="_nextPhase">次のフェーズ</param>
	void SetNextPhase(const GameScene::PHASE _nextPhase);

private:

	//パッドのカーソル位置
	const Vector2& padCursolPos_;

	//アイコン
	int* imgIcons_;

	//アイコンインデックス
	int iconIndex_;

	//座標
	Vector2 pos_;

	//次のフェーズ
	GameScene::PHASE nextPhase_;

	//アイコンをクリックしたか
	bool IsCheckChangePhase(GameScene& _parent);
};

