#pragma once
#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	static constexpr int IMG_ARCS_NUM = 5;		//円弧画像の数
	static constexpr int IMG_ARCS_DIV_X = 3;	//円弧画像の分割数X
	static constexpr int IMG_ARCS_DIV_Y = 2;	//円弧画像の分割数Y
	static constexpr int IMG_ARC_SIZE = 512;	//円弧画像の1分割したサイズ

	static constexpr int IMG_CURSORS_DIV_X = 4;	//分割数X
	static constexpr int IMG_CURSORS_DIV_Y = 1; //分割数Y
	static constexpr int IMG_CURSORS_SIZE = 64; //サイズ

	static constexpr int ITEM_ICONS_NUM = 13;	//アイコン種類数
	static constexpr int ITEM_ICONS_DIV_X = 5;	//アイコンスプライト分割数X
	static constexpr int ITEM_ICONS_DIV_Y = 3;	//アイコンスプライト分割数Y
	static constexpr int ITEM_ICON_SIZE = 128;	//アイテムアイコンの大きさ

	// リソース名
	enum class SRC
	{
		NONE,

		//画像
		PALETTE,
		SCROLL_ARROW_ICON,
		ITEM_ICONS,
		PALETTE_MASK,

		//タイトルシーン用画像
		TITLE_LOGO,

		//セレクトシーン用画像
		ARC,
		ARCS,
		BACK_ARC,
		SHADOW_ARC,
		CURSORS,

		//アイテムモデル
		FLOOR,				//足場
		MOVE_FLOOR,			//動く足場
		FENCE,				//柵
		BOMB,				//爆弾
		CANNON_BARREL,		//砲身
		CANNON_TURRET,		//砲台
		CANNON_SHOT,		//大砲の弾
		START_CUBE,			//スタート
		GOAL_CUBE,			//ゴール
		SPRING,				//ばね
		SPIKE,				//とげ
		SLIME_FLOOR,		//スライム床
		FAN,				//扇風機
		SKY_DOME,			//スカイドーム
		CHICKEN,			//チキンモデル
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void);

	// 内部ロード
	Resource& _Load(SRC src);

};
