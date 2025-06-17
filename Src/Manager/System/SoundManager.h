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

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SoundManager();

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
	/// 音源の再生
	/// </summary>
	/// <param name="_sound">音源</param>
	/// <param name="_playType">再生種類</param>
	void Play(const int& _sound, const PLAYTYPE _playType);

	/// <summary>
	/// 音源の停止
	/// </summary>
	/// <param name="_sound">音源</param>
	void Stop(const int& _sound);

	/// <summary>
	/// 音源の音量を変更
	/// </summary>
	/// <param name="_sound">音源</param>
	/// <param name="_volumeParcent">音量パーセント、最大100まで</param>
	void ChangeVolume(const int& _sound, const int _volumeParcent);

private:

	//静的インスタンス
	static SoundManager* instance_;

	// コンストラクタ
	SoundManager();

	// デフォルトコンストラクタは使用不可
	SoundManager(const SoundManager&) = delete;

	// コピー代入演算子は使用不可
	SoundManager& operator=(const SoundManager&) = delete;

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);

};

