#include "SoundManager.h"
#include <DxLib.h>

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

void SoundManager::ChangeVolume(const int _sound, const int _volumePercent)
{
	static constexpr int VOLUME_MAX = 255;  //最大音量
	static constexpr int DIV = 100;         //音量の割合を計算するための定数

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
