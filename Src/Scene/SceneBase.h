#pragma once
#include<functional>
#include "../Application.h"

class SceneManager;
class ResourceManager;
class InputManager;

class SceneBase
{

public:

	//最低ローディング時間
	static constexpr float LOADING_TIME = 2.0f;

	//フォントサイズ
	static constexpr int FONT_SIZE = 28;

	//ローディング
	static constexpr int COMMA_MAX_NUM = 7;											//「now loading......」のコンマの数
	static constexpr float COMMA_TIME = 0.5f;										//「now loading......」のコンマ数を増やす時間
	static constexpr int LOADING_STRING_POS_X = Application::SCREEN_SIZE_X - 300;	//「now loading......」の座標X
	static constexpr int LOADING_STRING_POS_Y = Application::SCREEN_SIZE_Y - 40;	//「now loading......」の座標Y

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void) = 0;

	// 読み込み処理
	virtual void Load(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual void Update(void);

	// 描画処理
	virtual void Draw(void);

protected:

	// リソース管理
	ResourceManager& resMng_;
	InputManager& inputMng_;
	SceneManager& scnMng_;

	//更新処理管理
	std::function<void(void)> updataFunc_;
	std::function<void(void)> drawFunc_;

	//ローディング経過時間
	float loadingTime_;

	//選択ボタンフォントハンドル
	int buttnFontHandle_;

	//更新関数
	virtual void LoadingUpdate(void);
	virtual void NormalUpdate(void) = 0;

	//描画関数
	virtual void LoadingDraw(void);
	virtual void NormalDraw(void) = 0;

	//ローディング処理から通常処理へ
	virtual void ChangeNormal(void) = 0;
	
	//「now loading......」の描画
	void DrawNowLoading(void);

};