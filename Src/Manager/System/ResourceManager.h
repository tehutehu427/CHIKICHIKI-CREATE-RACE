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

	static constexpr int PLAYER_PLATES_NUM = 4;	//プレイヤープレート種類数
	static constexpr int PLAYER_PLATES_DIV_X = 1;//プレイヤープレート分割数X
	static constexpr int PLAYER_PLATES_DIV_Y = 4;//プレイヤープレート分割数Y
	static constexpr int PLAYER_PLATES_SIZE_X = 512;//プレイヤープレートの大きさX
	static constexpr int PLAYER_PLATES_SIZE_Y = 128;//プレイヤープレートの大きさY

	static constexpr int CHANGE_PHASE_ICONS_NUM = 2;//フェーズ遷移用アイコン数
	static constexpr int CHANGE_PHASE_ICONS_DIV_X = 1;//フェーズ遷移用アイコン分割数X
	static constexpr int CHANGE_PHASE_ICONS_DIV_Y = 2;//フェーズ遷移用アイコン分割数Y
	static constexpr int CHANGE_PHASE_ICONS_SIZE_X = 160;//フェーズ遷移用アイコン大きさX
	static constexpr int CHANGE_PHASE_ICONS_SIZE_Y = 80;//フェーズ遷移用アイコン大きさY

	static constexpr int NUMBERS_DIV_X = 5;	//ナンバーの分割数X
	static constexpr int NUMBERS_DIV_Y = 2;	//ナンバーの分割数Y
	static constexpr int NUMBERS_SIZE = 128; //ナンバーの各画像サイズ

	static constexpr int SELECT_MES_DIV_X = 1;	//セレクトシーン用メッセージ分割数X
	static constexpr int SELECT_MES_DIV_Y = 5;	//セレクトシーン用メッセージ分割数Y
	static constexpr int SELECT_MES_SIZE_X = 1024; //セレクトシーン用メッセージ各画像サイズX
	static constexpr int SELECT_MES_SIZE_Y = 128; //セレクトシーン用メッセージ各画像サイズY

	static constexpr int CLEAR_MENUS_DIV_X = 1;//クリア時用メッセージ分割数X
	static constexpr int CLEAR_MENUS_DIV_Y = 3;//クリア時用メッセージ分割数Y
	static constexpr int CLEAR_MENUS_SIZE_X = 796;//クリア時用メッセージ各画像サイズX
	static constexpr int CLEAR_MENUS_SIZE_Y = 128;//クリア時用メッセージ各画像サイズY

	static constexpr int SMOKE_EFE_DIV_X = 16;	//煙エフェクト分割数X
	static constexpr int SMOKE_EFE_DIV_Y = 1;	//煙エフェクト分割数Y
	static constexpr int SMOKE_EFE_SIZE = 64;	//煙エフェクトサイズ

	static constexpr int LEVEL_SLE_DIV_X = 2;		//レベル分割数X
	static constexpr int LEVEL_SLE_DIV_Y = 2;		//レベル分割数Y
	static constexpr int LEVEL_SLE_SIZE_X = 400;	//レベルサイズ
	static constexpr int LEVEL_SLE_SIZE_Y = 300;	//レベルサイズ

	static constexpr int YES_NO_DIV_X = 1;		//YesNo分割数X
	static constexpr int YES_NO_DIV_Y = 2;		//YesNo分割数Y
	static constexpr int YES_NO_SIZE_X = 192;	//YesNoサイズX
	static constexpr int YES_NO_SIZE_Y = 64;	//YesNoサイズY

	static constexpr int EDIT_MESSAGES_DIV_X = 1;	//エディットメッセージ分割数X
	static constexpr int EDIT_MESSAGES_DIV_Y = 3;	//エディットメッセージ分割数Y
	static constexpr int EDIT_MESSAGES_SIZE_X = 980; //エディットメッセージ各画像サイズX
	static constexpr int EDIT_MESSAGES_SIZE_Y = 128; //エディットメッセージ各画像サイズY

	static constexpr int SYSTEM_MESSAGES_DIV_X = 1;	//システムメッセージ分割数X
	static constexpr int SYSTEM_MESSAGES_DIV_Y = 2;	//システムメッセージ分割数Y
	static constexpr int SYSTEM_MESSAGES_SIZE_X = 286; //システムメッセージ各画像サイズX
	static constexpr int SYSTEM_MESSAGES_SIZE_Y = 64; //システムメッセージ各画像サイズY

	static constexpr int SYSTEM_SETTING_MESSAGES_DIV_X = 1;	//システム設定メッセージ分割数X
	static constexpr int SYSTEM_SETTING_MESSAGES_DIV_Y = 5;	//システム設定メッセージ分割数Y
	static constexpr int SYSTEM_SETTING_MESSAGES_SIZE_X = 544; //システム設定メッセージ各画像サイズX
	static constexpr int SYSTEM_SETTING_MESSAGES_SIZE_Y = 48; //システム設定メッセージ各画像サイズY

	static constexpr int ONOFF_DIV_X = 1; //有無の分割数X
	static constexpr int ONOFF_DIV_Y = 2; //有無の分割数Y
	static constexpr int ONOFF_SIZE_X = 128; //有無の各画像サイズX
	static constexpr int ONOFF_SIZE_Y = 64; //有無の各画像サイズY

	static constexpr int PLAYER_GAGES_DIV_X = 1; //プレイヤーゲージの分割数X
	static constexpr int PLAYER_GAGES_DIV_Y = 4; //プレイヤーゲージの分割数Y
	static constexpr int PLAYER_GAGES_SIZE_X = 600; //プレイヤーゲージの各画像サイズX
	static constexpr int PLAYER_GAGES_SIZE_Y = 50; //プレイヤーゲージの各画像サイズY

	// リソース名
	enum class SRC
	{
		NONE,

		//エディット画像
		PALETTE,
		SCROLL_ARROW_ICON,
		ITEM_ICONS,
		PALETTE_MASK,
		SAVE_ICON,
		LOAD_ICON,
		MANUAL_ICON,
		EXPLAN_BACK,
		CHANGE_PHASE_ICONS,
		CANCEL_ICON,
		READY_IMG,
		EDIT_MESSAGES,
		SYSTEM_MESSAGES,
		YES_NO,
		ERROR_MAP_OUT_IMG,
		ERROR_OVERLAP_IMG,
		ERROR_CANT_ITEM_IMG,
		KEY_EXPLANATION,
		PAD_EXPLANATION,

		//クリア
		CLEAR,
		CLEAR_MENUS,
		PLAYER_PLATES,
		WIN,

		//ゲームオーバー
		GAMEOVER,

		//タイトルシーン用画像
		TITLE_LOGO,
		PUSHSPACE,
		PUSH_DEMO,

		//セレクトシーン用画像
		ARC,
		ARCS,
		BACK_ARC,
		SHADOW_ARC,
		CURSORS,
		SELECT_MESSAGES,
		LEVEL_SELECTS,
		SYSTEM_SETTING,
		SETTING_ITEMS,
		PUSH_B_BUTTON_MES,
		ONOFF,
		SETTING_FINISH,
		IS_WINNING_MES,


		//ゲームシーン用画像
		NUMBERS,
		OK,
		ROUND,
		PROGRESS,
		SELECT_ITEM,
		PLAYER_GAGES,
		PLAYER_GAGE_OUTLINE,
		SELECT_SKIP,
		DRAW_MES,
		RESULT_GOAL,
		RESULT_NO1,
		RESULT_COIN,

		//マニュアル
		FREE_MANUAL,
		FREE_MANUAL_2,
		FREE_MANUAL_3,
		SELECT_MANUAL_KEY,
		SELECT_MANUAL_PAD,


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
		COL_CUBE,			//当たり判定用箱
		SPRING,				//ばね
		SPIKE,				//とげ
		SLIME_FLOOR,		//スライム床
		FAN,				//扇風機
		COIN,
		SKY_DOME,			//スカイドーム
		CHICKEN,			//チキンモデル		
		
		//2Dエフェクト
		SMOKE_EFE_SPR,		//煙エフェクト

		//エフェクト
		WIND_EFF,			//風エフェクト
		BLAST_EFF,			//爆発エフェクト
		DASH_EFF,			//ダッシュエフェクト
		JUMP_EFF,			//ジャンプエフェクト
		LANDING_EFF,		//着地エフェクト
		PUNCH_HIT_EFF,		//パンチヒットエフェクト
		COIN_GOAL_EFF,		//コインのゴールエフェクト



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
