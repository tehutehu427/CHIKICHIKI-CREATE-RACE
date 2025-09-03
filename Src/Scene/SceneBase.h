#pragma once
#include<functional>
#include "../Application.h"
#include "../Manager/System/ResourceManager.h"

class SceneManager;
class ResourceManager;
class SoundManager;
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

	// 共通の描画処理
	virtual void CommonDraw(void) {};

protected:

	// リソース管理
	ResourceManager& resMng_;
	KeyConfig& keyConfig_;
	SceneManager& scnMng_;
	SoundManager& sndMng_;

	//処理の管理
	ProcessFunction func_;

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

	//ランダムでローディングメッセージを読み込む
	void LoadRandomLoadingMessage(void);
	
private:

	//リソース種類数
	static constexpr int LOADING_RESOURCES = 5;

	//拡大率
	static constexpr float RATE = 1.3f;

	//ローディング用リソース
	static constexpr ResourceManager::SRC LOADING_RESOURCE_TYPE[LOADING_RESOURCES] =
	{
		ResourceManager::SRC::LOADING_MESS_ABYSS,
		ResourceManager::SRC::LOADING_MESS_BOMB,
		ResourceManager::SRC::LOADING_MESS_CANNON,
		ResourceManager::SRC::LOADING_MESS_COIN,
		ResourceManager::SRC::LOADING_MESS_STICK
	};	

	//ローディング中の画像描画
	int imgLoadingMessage_;
	
	//「now loading......」の描画
	void DrawNowLoading(void);

	//メッセージの描画
	void DrawLoadingMessage(void);
};