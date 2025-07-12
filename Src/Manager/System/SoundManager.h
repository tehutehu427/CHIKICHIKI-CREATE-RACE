#pragma once
#include <unordered_map>
#include <string>

class SoundManager
{
public:

	/// <summary>
	/// リソース種類
	/// </summary>
	enum class SRC
	{
		TITLE_BGM,			//タイトル
	};

	/// <summary>
	/// 音源種類
	/// </summary>
	enum class TYPE
	{
		BGM,				//BGM
		SE,					//効果音
	};

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
	/// リソースの解放
	/// </summary>
	void Release();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 読み込み
	/// </summary>
	const int LoadResource(const SRC _src);

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

	struct SoundResource
	{
		int handleId;		//音源ハンドルID
		TYPE type;			//音源の種類
		std::string path;	//音源のパス
	};

	//管理対象
	std::unordered_map<SRC, SoundResource> resourcesMap_;

	//読み込み済み
	std::unordered_map<SRC, int&> loadedMap_;

	// コンストラクタ
	SoundManager();

	// デフォルトコンストラクタは使用不可
	SoundManager(const SoundManager&) = delete;

	// コピー代入演算子は使用不可
	SoundManager& operator=(const SoundManager&) = delete;

	//内部読み込み処理
	int _Load(const SRC _src);

	//音量を調整
	void ChangeVolume(const int _sound, const int _volumeParcent);

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);

};

