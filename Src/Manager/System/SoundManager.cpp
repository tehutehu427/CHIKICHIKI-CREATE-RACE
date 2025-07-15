#include "SoundManager.h"
#include <DxLib.h>
#include <cassert>
#include "../../Application.h"

SoundManager* SoundManager::instance_ = nullptr;

SoundManager::SoundManager()
{  
	// 音量の初期化
	for (int i = 0; i < TYPE_MAX; ++i)
	{
		volume_[i] = DEFAULT_VOLUME;
	}
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
    instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
    return *instance_;
}

void SoundManager::Destroy()
{
    Release();
	resourcesMap_.clear();
    if (instance_ != nullptr)
    {
        delete instance_;
        instance_ = nullptr;
    }
}

void SoundManager::Release()
{
    for (auto& p : loadedMap_)
    {
        DeleteSoundMem(p.second.handleId);
    }

    loadedMap_.clear();
}

void SoundManager::Init()
{
    SoundResource res = { -1, TYPE::BGM,"" };
	std::string path_Bgm = Application::PATH_SOUND_BGM;
	std::string path_Se = Application::PATH_SOUND_SE;

#pragma region BGM

    res.path = path_Bgm  + "TitleBgm.wav";
    resourcesMap_.emplace(SRC::TITLE_BGM, res);

    res.path = path_Bgm + "SelectBgm.wav";
    resourcesMap_.emplace(SRC::SELECT_BGM, res);

    res.path = path_Bgm + "EditBgm.mp3";
    resourcesMap_.emplace(SRC::EDIT_BGM, res);

    res.path = path_Bgm + "PlayBgm.mp3";
    resourcesMap_.emplace(SRC::PLAY_BGM_1, res);

    res.path = path_Bgm + "PlayBgm2.mp3";
    resourcesMap_.emplace(SRC::PLAY_BGM_2, res);

    res.path = path_Bgm  + "MultiBgm.mp3";
    resourcesMap_.emplace(SRC::MULTI_BGM_1, res);

    res.path = path_Bgm  + "MultiBgm2.mp3";
    resourcesMap_.emplace(SRC::MULTI_BGM_2, res);

	res.path = path_Bgm + "MultiClearBgm.mp3";
    resourcesMap_.emplace(SRC::MULTI_CLEAR_BGM, res);

#pragma endregion

#pragma region SE
	res.type = TYPE::SE;

    res.path = path_Se + "PunchHit.mp3";
    resourcesMap_.emplace(SRC::PLAYER_PUNCH_HIT, res);

    res.path = path_Se + "PunchMotion.mp3";
    resourcesMap_.emplace(SRC::PLAYER_PUNCH_MOTION, res);

    res.path = path_Se + "Dash.mp3";
    resourcesMap_.emplace(SRC::PLAYER_DASH_START, res);

    res.path = path_Se + "Jump.mp3";
    resourcesMap_.emplace(SRC::PLAYER_JUMP, res);

    res.path = path_Se + "Spring.mp3";
    resourcesMap_.emplace(SRC::SPRING_SE, res);

    res.path = path_Se + "SlimeFloor.mp3";
    resourcesMap_.emplace(SRC::SLIME_SE, res);

    res.path = path_Se + "ClickItem.ogg";
    resourcesMap_.emplace(SRC::CLICK_OBJECT_SE, res);

    res.path = path_Se + "CreateItem.ogg";
    resourcesMap_.emplace(SRC::CREATE_OBJECT_SE, res);

    res.path = path_Se + "Error.mp3";
    resourcesMap_.emplace(SRC::ERROR_SE, res);

	res.path = path_Se + "ChickenVoice1.mp3";
	resourcesMap_.emplace(SRC::CHICKEN_SE, res);

	res.path = path_Se + "ChickenVoice2.mp3";
	resourcesMap_.emplace(SRC::CHICKEN_SE_2, res);

	res.path = path_Se + "ChickenVoice3.mp3";
	resourcesMap_.emplace(SRC::CHICKEN_SE_3, res);

	res.path = path_Se + "ChickenVoice4.mp3";
	resourcesMap_.emplace(SRC::CHICKEN_SE_4, res);

	res.path = path_Se + "TitleSceneChange.mp3";
	resourcesMap_.emplace(SRC::TITLE_SCENE_CHANGE, res);

    res.path = path_Se + "SelectSceneChange.mp3";
    resourcesMap_.emplace(SRC::SELECT_SCENE_CHANGE, res);

	res.path = path_Se + "Decision.mp3";
	resourcesMap_.emplace(SRC::DECISION, res);

    res.path = path_Se + "Cancel.mp3";
    resourcesMap_.emplace(SRC::CANCEL, res);

	res.path = path_Se + "Ok.mp3";
	resourcesMap_.emplace(SRC::OK, res);

	res.path = path_Se + "Delon.mp3";
	resourcesMap_.emplace(SRC::DELON, res);

	res.path = path_Se + "RoundJingle.mp3";
	resourcesMap_.emplace(SRC::ROUND_JINGLE, res);

	res.path = path_Se + "PaletteClick.mp3";
	resourcesMap_.emplace(SRC::PALETTE_CLICK, res);

	res.path = path_Se + "PaletteOpen.mp3";
	resourcesMap_.emplace(SRC::PALETTE_OPEN, res);

	res.path = path_Se + "PaletteClose.mp3";
	resourcesMap_.emplace(SRC::PALETTE_CLOSE, res);

	res.path = path_Se + "EditSystemIconClick.mp3";
	resourcesMap_.emplace(SRC::EDIT_SYSTEM_ICON_CLICK, res);

	res.path = path_Se + "ChangePhaseIcon.mp3";
	resourcesMap_.emplace(SRC::CHANGE_PHASE_ICON_CLICK, res);

	res.path = path_Se + "CountDown.mp3";
	resourcesMap_.emplace(SRC::COUNTDOWN_SE, res);

	res.path = path_Se + "DrumRoll.mp3";
	resourcesMap_.emplace(SRC::DRUM_ROLL, res);

	res.path = path_Se + "DrumRollEnd.mp3";
	resourcesMap_.emplace(SRC::DRUM_ROLL_END, res);

	res.path = path_Se + "MultiClearJingle.mp3";
	resourcesMap_.emplace(SRC::MULTI_CLEAR_JINGLE, res);

	res.path = path_Se + "GameOver.mp3";
	resourcesMap_.emplace(SRC::GAME_OVER, res);

	res.path = path_Se + "Clear.mp3";
	resourcesMap_.emplace(SRC::CLEAR, res);

	res.path = path_Se + "Bomb.mp3";
	resourcesMap_.emplace(SRC::BOMB_SE, res);
#pragma endregion

}

const bool SoundManager::LoadResource(const SRC _src)
{
    return _Load(_src);
}

void SoundManager::Play(const SRC _src, const PLAYTYPE _playType)
{
	//音源が読み込まれていない場合はエラー
	assert(loadedMap_[_src].handleId != -1, "音源が読み込まれてないです");

    //音源が再生済みか調べる
	if (CheckSoundMem(loadedMap_[_src].handleId) == 1 &&
        _playType != PLAYTYPE::BACK)
	{
		Stop(_src);  // 再生済みなら停止
	}

    //音源の再生
    PlaySoundMem(loadedMap_[_src].handleId, GetPlayType(_playType));
}

void SoundManager::Stop(const SRC _src)
{
    //音源の停止
    StopSoundMem(loadedMap_[_src].handleId);
}

bool SoundManager::IsPlay(const SRC _src) const
{
    const auto it = loadedMap_.find(_src);
    if (it == loadedMap_.end())
    {
        return false; // 見つからない場合は未再生とする
    }
    return CheckSoundMem(it->second.handleId) == 1;
}

void SoundManager::SetSystemVolume(const int _volumePercent, const int _type)
{    
    constexpr int VOLUME_MAX = 255;  //最大音量
    constexpr int DIV = 100;         //音量の割合を計算するための定数

    //音量設定
    volume_[_type] = _volumePercent;
   
    //音量調整
	for (const auto& pair : loadedMap_)
	{
        //種類が異なるものはスキップ
		if (pair.second.type != static_cast<TYPE>(_type)) 
        {
			continue;
		}
        ChangeVolumeSoundMem(VOLUME_MAX * volume_[_type] / DIV, pair.second.handleId);
	}
}

bool SoundManager::_Load(const SRC _src)
{
    // ロード済みチェック
    const auto& lPair = loadedMap_.find(_src);
    if (lPair != loadedMap_.end())
    {
		return false;   // 既にロード済み
    }

    // リソース登録チェック
    const auto& rPair = resourcesMap_.find(_src);
    if (rPair == resourcesMap_.end())
    {
        return false;   // 登録されていない
    }

    // ロード処理
    rPair->second.handleId = LoadSoundMem(rPair->second.path.c_str());

    // 念のためコピーコンストラクタ
    loadedMap_.emplace(_src, rPair->second);

    return true;
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
