#pragma once
class SoundManager
{
public:

	/// <summary>
	/// 再生種類
	/// </summary>
	enum class PLAYTYPE
	{
		NORMAL,	//ノーマル再生
		LOOP,	//ループ再生
		BACK	//バックグラウンド再生
	};

	//デフォルトの音量
	static constexpr int DEFAULT_VOLUME = 70;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SoundManager();

	/// <summary>
	/// 明示的にインスタンス生成する
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// 静的インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>インスタンス
	static SoundManager& GetInstance(void);

	/// <summary>
	/// 解放
	/// </summary>
	void Destroy();

	/// <summary>
	/// 音源の再生
	/// </summary>
	/// <param name="_sound">音源</param>
	/// <param name="_playType">再生種類</param>
	void Play(const int _sound, const PLAYTYPE _playType, const int _volumePercent = -1);

	/// <summary>
	/// 音源の停止
	/// </summary>
	/// <param name="_sound">音源</param>
	void Stop(const int _sound);

	/// <summary>
	/// 音量の設定
	/// </summary>
	/// <param name="_volumePercent">音量パーセント</param>
	void SetVolume(const int _volumePercent) { volume_ = _volumePercent; }

private:

	//静的インスタンス
	static SoundManager* instance_;

	//ボリューム
	int volume_;

	// コンストラクタ
	SoundManager();

	// デフォルトコンストラクタは使用不可
	SoundManager(const SoundManager&) = delete;

	// コピー代入演算子は使用不可
	SoundManager& operator=(const SoundManager&) = delete;

	//音量を調整
	void ChangeVolume(const int _sound, const int _volumeParcent);

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);

};

