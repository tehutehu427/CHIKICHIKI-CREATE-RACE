#include "SoundManager.h"
#include <DxLib.h>

SoundManager* SoundManager::instance_ = nullptr;

SoundManager::SoundManager()
{  
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
    // ‰Šú‰»ˆ—‚ðŒÄ‚Ño‚·
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

void SoundManager::Play(const int _sound, const PLAYTYPE _playType)
{
    PlaySoundMem(_sound, GetPlayType(_playType));
}

void SoundManager::Stop(const int _sound)
{
    StopSoundMem(_sound);
}

void SoundManager::ChangeVolume(const int _sound, const int _volumeParcent)
{
    static constexpr int VOLUME_MAX = 255;
    static constexpr int DIV = 100;
    ChangeVolumeSoundMem(VOLUME_MAX * _volumeParcent / DIV, _sound);
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
