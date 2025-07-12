#include "SoundManager.h"
#include <DxLib.h>
#include "../../Application.h"

SoundManager* SoundManager::instance_ = nullptr;

SoundManager::SoundManager()
{  
	// 音量の初期化
    volume_ = DEFAULT_VOLUME;
}

SoundManager::~SoundManager()
{
}

void SoundManager::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new SoundManager();
    }
    // 初期化処理を呼び出す
    //instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
    return *instance_;
}

void SoundManager::Destroy()
{
    if (instance_ != nullptr)
    {
        delete instance_;
        instance_ = nullptr;
    }
}

void SoundManager::Release()
{
}

void SoundManager::Init()
{
    SoundResource res = { -1, TYPE::BGM,"" };
	std::string path_Bgm = Application::PATH_SOUND_BGM;
	std::string path_Se = Application::PATH_SOUND_SE;

#pragma region BGM
    //タイトルBGM
	res.path = path_Bgm + "TitleBgm.mp3";
	resourcesMap_.emplace(SRC::TITLE_BGM, res);
#pragma endregion

#pragma region SE
	res.type = TYPE::SE;


#pragma endregion

}

const int SoundManager::LoadResource(const SRC _src)
{
    return  _Load(_src);
}

void SoundManager::Play(const int _sound, const PLAYTYPE _playType, const int _volumePercent)
{
    //音量調整
    ChangeVolume(_sound, _volumePercent);

	//音源の再生
    PlaySoundMem(_sound, GetPlayType(_playType));
}

void SoundManager::Stop(const int _sound)
{
	//音源の停止
    StopSoundMem(_sound);
}

int SoundManager::_Load(const SRC _src)
{
    // ロード済みチェック
    const auto& lPair = loadedMap_.find(_src);
    if (lPair != loadedMap_.end())
    {
        return lPair->second;
    }

    // リソース登録チェック
    const auto& rPair = resourcesMap_.find(_src);
    if (rPair == resourcesMap_.end())
    {
        // 登録されていない
        return -1;
    }

    // ロード処理
    rPair->second.handleId = LoadSoundMem(rPair->second.path.c_str());

    // 念のためコピーコンストラクタ
    loadedMap_.emplace(_src, rPair->second.handleId);

    return rPair->second.handleId;
}

void SoundManager::ChangeVolume(const int _sound, const int _volumePercent)
{
	constexpr int VOLUME_MAX = 255;  //最大音量
    constexpr int DIV = 100;         //音量の割合を計算するための定数

	//音量パーセントが-1の場合は、デフォルトの音量を使用
    int volumePercent = _volumePercent <= -1 ? volume_ : _volumePercent;

    //音量調整
    ChangeVolumeSoundMem(VOLUME_MAX * volumePercent / DIV, _sound);
}

int SoundManager::GetPlayType(const PLAYTYPE _playType)
{
    switch (_playType)
    {
    case PLAYTYPE::NORMAL:
        return DX_PLAYTYPE_NORMAL;
        break;

    case PLAYTYPE::LOOP:
        return DX_PLAYTYPE_LOOP;
        break;

    case PLAYTYPE::BACK:
        return DX_PLAYTYPE_BACK;
        break;

    default:
        return DX_PLAYTYPE_NORMAL;
        break;
    }
}
