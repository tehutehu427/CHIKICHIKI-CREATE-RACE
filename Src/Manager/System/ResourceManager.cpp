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
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

#pragma region 画像
	std::string path_EditUi = Application::PATH_IMAGE + "EditUI/";
	std::string path_PlayUi = Application::PATH_IMAGE + "PlayUI/";
	std::string path_RezaltUi = Application::PATH_IMAGE + "RezaltUI/";
	std::string path_Title = Application::PATH_IMAGE + "Title/";
	std::string path_Select = Application::PATH_IMAGE + "Select/";

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "ScrollArrowIcon.png");
	resourcesMap_.emplace(SRC::SCROLL_ARROW_ICON, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "Palette.png");
	resourcesMap_.emplace(SRC::PALETTE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_EditUi + "PaletteIcons.png");
	resourcesMap_.emplace(SRC::PALETTE_ICONS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_Select + "Arcs.png", IMG_ARCS_DIV_X, IMG_ARCS_DIV_Y, IMG_ARC_SIZE, IMG_ARC_SIZE);
	resourcesMap_.emplace(SRC::ARCS, std::move(res));
	
	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Select + "BackArc.png");
	resourcesMap_.emplace(SRC::BACK_ARC, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Select + "ShadowArc.png");
	resourcesMap_.emplace(SRC::SHADOW_ARC, std::move(res));

#pragma endregion 

#pragma region マスク画像
	std::string path_Mask = Application::PATH_IMAGE + "Mask/";
	res = std::make_unique<Resource>(Resource::TYPE::MASK, path_Mask + "PaletteMask.png");
	resourcesMap_.emplace(SRC::PALETTE_MASK, std::move(res));
#pragma endregion 

#pragma region モデル
	std::string path_Sky = PATH_MDL + "SkyDome/";
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Floor.mv1");
	resourcesMap_.emplace(SRC::FLOOR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "MoveFloor.mv1");
	resourcesMap_.emplace(SRC::MOVE_FLOOR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Fence.mv1");
	resourcesMap_.emplace(SRC::MOVE_FLOOR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Bomb.mv1");
	resourcesMap_.emplace(SRC::BOMB, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "CannonBarrel.mv1");
	resourcesMap_.emplace(SRC::CANNON_BARREL, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "CannonTurret.mv1");
	resourcesMap_.emplace(SRC::CANNON_TURRET, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "CannonShot.mv1");
	resourcesMap_.emplace(SRC::CANNON_SHOT, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Spring.mv1");
	resourcesMap_.emplace(SRC::SPRING, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "SlimeFloor.mv1");
	resourcesMap_.emplace(SRC::SLIME_FLOOR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, path_Sky + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + "Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, std::move(res));
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
