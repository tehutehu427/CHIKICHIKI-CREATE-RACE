#include <DxLib.h>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	std::string PATH_IMG = Application::PATH_IMAGE;
	std::string PATH_MDL = Application::PATH_MODEL;
	std::string PATH_EFF = Application::PATH_EFFECT;
	std::string PATH_SOUND_BGM = Application::PATH_SOUND_BGM;
	std::string PATH_SOUND_SE = Application::PATH_SOUND_SE;

	std::unique_ptr<Resource> res;

#pragma region 画像
	std::string path_EditUi = Application::PATH_IMAGE + "EditUI/";
	std::string path_Title = Application::PATH_IMAGE + "Title/";
	std::string path_Select = Application::PATH_IMAGE + "Select/";
	std::string path_Clear = Application::PATH_IMAGE + "Clear/";
	std::string path_GameOver = Application::PATH_IMAGE + "GameOver/";
	std::string path_Game = Application::PATH_IMAGE + "Game/";
	std::string path_Effect = Application::PATH_IMAGE + "Effect/";

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "ScrollArrowIcon.png");
	resourcesMap_.emplace(SRC::SCROLL_ARROW_ICON, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "Palette.png");
	resourcesMap_.emplace(SRC::PALETTE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_EditUi + "ItemIcons.png", ITEM_ICONS_DIV_X,ITEM_ICONS_DIV_Y,ITEM_ICON_SIZE,ITEM_ICON_SIZE);
	resourcesMap_.emplace(SRC::ITEM_ICONS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "LoadIcon.png");
	resourcesMap_.emplace(SRC::LOAD_ICON, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "ManualIcon.png");
	resourcesMap_.emplace(SRC::MANUAL_ICON, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "SaveIcon.png");
	resourcesMap_.emplace(SRC::SAVE_ICON, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "CancelIcon.png");
	resourcesMap_.emplace(SRC::CANCEL_ICON, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "Ready.png");
	resourcesMap_.emplace(SRC::READY_IMG, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_EditUi + "ChangePhaseIcons.png", CHANGE_PHASE_ICONS_DIV_X, CHANGE_PHASE_ICONS_DIV_Y, CHANGE_PHASE_ICONS_SIZE_X, CHANGE_PHASE_ICONS_SIZE_Y);
	resourcesMap_.emplace(SRC::CHANGE_PHASE_ICONS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "ExplanBack.png");
	resourcesMap_.emplace(SRC::EXPLAN_BACK, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_EditUi + "Cursors.png",IMG_CURSORS_DIV_X, IMG_CURSORS_DIV_Y, IMG_CURSORS_SIZE, IMG_CURSORS_SIZE);
	resourcesMap_.emplace(SRC::CURSORS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_Select + "SelectArcs.png", IMG_ARCS_DIV_X, IMG_ARCS_DIV_Y, IMG_ARC_SIZE, IMG_ARC_SIZE);
	resourcesMap_.emplace(SRC::ARCS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_Select + "SelectMessages.png", SELECT_MES_DIV_X, SELECT_MES_DIV_Y, SELECT_MES_SIZE_X, SELECT_MES_SIZE_Y);
	resourcesMap_.emplace(SRC::SELECT_MESSAGES, std::move(res));
	
	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Select + "BackArc.png");
	resourcesMap_.emplace(SRC::BACK_ARC, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Select + "ShadowArc.png");
	resourcesMap_.emplace(SRC::SHADOW_ARC, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_Select + "LevelSelects.png", LEVEL_SLE_DIV_X, LEVEL_SLE_DIV_Y, LEVEL_SLE_SIZE_X, LEVEL_SLE_SIZE_Y);
	resourcesMap_.emplace(SRC::LEVEL_SELECTS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Title + "TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Title + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSHSPACE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_GameOver + "GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Clear + "Clear.png");
	resourcesMap_.emplace(SRC::CLEAR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Clear + "Win.png");
	resourcesMap_.emplace(SRC::WIN, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_Clear + "ClearSelectMenus.png", CLEAR_MENUS_DIV_X, CLEAR_MENUS_DIV_Y, CLEAR_MENUS_SIZE_X, CLEAR_MENUS_SIZE_Y);;
	resourcesMap_.emplace(SRC::CLEAR_MENUS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_Clear + "PlayerPlates.png", PLAYER_PLATES_DIV_X, PLAYER_PLATES_DIV_Y, PLAYER_PLATES_SIZE_X, PLAYER_PLATES_SIZE_Y);;
	resourcesMap_.emplace(SRC::PLAYER_PLATES, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_Game + "Numbers.png", NUMBERS_DIV_X, NUMBERS_DIV_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Game + "Ok.png");
	resourcesMap_.emplace(SRC::OK, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Game + "Round.png");
	resourcesMap_.emplace(SRC::ROUND, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_Effect + "SmokeSprites.png", SMOKE_EFE_DIV_X, SMOKE_EFE_DIV_Y, SMOKE_EFE_SIZE, SMOKE_EFE_SIZE);
	resourcesMap_.emplace(SRC::SMOKE_EFE_SPR, std::move(res));
#pragma endregion 

#pragma region マスク画像
	std::string path_Mask = Application::PATH_IMAGE + "Mask/";
	res = std::make_unique<Resource>(Resource::TYPE::MASK, path_Mask + "PaletteMask.png");
	resourcesMap_.emplace(SRC::PALETTE_MASK, std::move(res));
#pragma endregion 

#pragma region モデル
	std::string path_Sky = PATH_MDL + "SkyDome/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "StartItem.mv1");
	resourcesMap_.emplace(SRC::START_CUBE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "GoalItem.mv1");
	resourcesMap_.emplace(SRC::GOAL_CUBE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "ColCube.mv1");
	resourcesMap_.emplace(SRC::COL_CUBE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Floor.mv1");
	resourcesMap_.emplace(SRC::FLOOR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "MoveFloor.mv1");
	resourcesMap_.emplace(SRC::MOVE_FLOOR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Fence.mv1");
	resourcesMap_.emplace(SRC::FENCE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Bomb.mv1");
	resourcesMap_.emplace(SRC::BOMB, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "CannonBarrel.mv1");
	resourcesMap_.emplace(SRC::CANNON_BARREL, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "CannonTurret.mv1");
	resourcesMap_.emplace(SRC::CANNON_TURRET, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "CannonShot.mv1");
	resourcesMap_.emplace(SRC::CANNON_SHOT, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Trap.mv1");
	resourcesMap_.emplace(SRC::SPIKE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Fan.mv1");
	resourcesMap_.emplace(SRC::FAN, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Spring.mv1");
	resourcesMap_.emplace(SRC::SPRING, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "SlimeFloor.mv1");
	resourcesMap_.emplace(SRC::SLIME_FLOOR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, path_Sky + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, std::move(res));
#pragma endregion 

#pragma region サウンド
	//BGM
	//--------------------------------------------------------------------
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_BGM + "TitleBgm.wav");
	resourcesMap_.emplace(SRC::TITLE_BGM, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_BGM + "MultiGameBgm1.mp3");
	resourcesMap_.emplace(SRC::MULTI_GAME_BGM_1, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_BGM + "MultiGameBgm2.mp3");
	resourcesMap_.emplace(SRC::MULTI_GAME_BGM_2, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_BGM + "MultiGameBgm3.mp3");
	resourcesMap_.emplace(SRC::MULTI_GAME_BGM_3, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_BGM + "MultiGameBgm3.mp3");
	resourcesMap_.emplace(SRC::MULTI_GAME_BGM_3, std::move(res));

	//SE
	//--------------------------------------------------------------------
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_SE + "PunchHit.mp3");
	resourcesMap_.emplace(SRC::PLAYER_PUNCH_HIT, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_SE + "PunchMotion.mp3");
	resourcesMap_.emplace(SRC::PLAYER_PUNCH_MOTION, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_SE + "dash.mp3");
	resourcesMap_.emplace(SRC::PLAYER_DASH_START, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_SE + "jump.mp3");
	resourcesMap_.emplace(SRC::PLAYER_JUMP, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_SE + "Spring.mp3");
	resourcesMap_.emplace(SRC::SPRING_SE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, PATH_SOUND_SE + "SlimeFloor.mp3");
	resourcesMap_.emplace(SRC::SLIME_SE, std::move(res));

	
#pragma endregion 

#pragma region エフェクト
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, PATH_EFF + "Wind.efkefc");
	resourcesMap_.emplace(SRC::WIND_EFF, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, PATH_EFF + "Blast.efkefc");
	resourcesMap_.emplace(SRC::BLAST_EFF, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, PATH_EFF + "Dash.efkefc");
	resourcesMap_.emplace(SRC::DASH_EFF, std::move(res));
#pragma endregion

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
