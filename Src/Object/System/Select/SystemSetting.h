#pragma once
#include <unordered_map>
#include <functional>

class SelectScene;

class SystemSetting
{
public:

	enum class STATE
	{
		CLEAR_SCORE,	//クリアスコア設定
		SKIP,			//スキップ設定
		SOUND_VOLUME,	//サウンドボリューム設定
		MAX				//最大値
	};
	
	//状態の最大値
	static constexpr int STATE_MAX = static_cast<int>(STATE::MAX);	
	
	//クリアスコアの最大値
	static constexpr int CLEAR_SCORE_MAX = 15;	

	//サウンドボリュームの最大値
	static constexpr int SOUND_VOLUME_MAX = 100;

	//サウンドの増加量
	static constexpr int SOUND_VOLUME_STEP = 10;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SystemSetting();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SystemSetting();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(SelectScene& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//画像
	int imgTriangle_;		//三角画像
	int* imgCursor_;		//カーソル画像
	int* imgSelectMess_;	//メッセージ
	int* imgSettings_;		//設定項目
	int* imgNumbers_;		//ナンバー画像

	//ステートインデックス
	int stateIndex_;

	//クリアスコア
	int clearScore_;	

	//サウンドボリューム
	int soundVolume_;	
	
	//スキップ設定
	bool isSkip_;
	
	//状態別右の処理の登録
	std::unordered_map<STATE, std::function<void()>> rightStateMap_; 

	//状態別左の処理の登録
	std::unordered_map<STATE, std::function<void()>> leftStateMap_; 

	//描画処理の登録
	std::unordered_map<STATE, std::function<void()>> drawStateMap_; 

	//データを反映
	void ApplyData();

	//矢印の描画
	void DrawCursor();

	//三角の描画
	void DrawSelectTriangle();

	//項目名の描画
	void DrawSelectStateName(const int _index);

	//状態別の描画
	void DrawClearScore();
	void DrawSkip();
	void DrawSoundVolume();
};

