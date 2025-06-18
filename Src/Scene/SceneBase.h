#pragma once
#include<functional>
#include "../Application.h"

class SceneManager;
class ResourceManager;
class KeyConfig;

class SceneBase
{

public:	
	
	//処理管理まとめ
	struct ProcessFunction
	{
		std::function<void()> updataFunc_;		//更新処理管理
		std::function<void()> drawFunc_;		//描画処理管理
	};

	//最低ローディング時間
	static constexpr float LOADING_TIME = 0.01f;

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
	KeyConfig& keyConfig_;
	SceneManager& scnMng_;
	KeyConfig& keyConfig_;

	//処理の管理
	ProcessFunction func_;

	//ローディング経過時間
	float loadingTime_;

	//選択ボタンフォントハンドル
	int buttnFontHandle_;

	//更新関数
	virtual void LoadingUpdate();
	virtual void NormalUpdate() = 0;

	//描画関数
	virtual void LoadingDraw();
	virtual void NormalDraw() = 0;

	//ローディング処理から通常処理へ
	virtual void ChangeNormal() = 0;
	
	//「now loading......」の描画
	void DrawNowLoading();

};