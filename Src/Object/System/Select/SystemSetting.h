#pragma once
#include <unordered_map>
#include <functional>
#include "../../../Manager/System/SoundManager.h"

class SelectScene;

class SystemSetting
{
public:

	enum class STATE
	{
		CLEAR_SCORE,	//クリアスコア設定
		SKIP,			//スキップ設定
		BGM_VOLUME,		//BGMボリューム設定
		SE_VOLUME,		//SEボリューム設定
		FULL_SCREEN,	//フルスクリーン		
		APPLY,			//設定適用
		MAX				//最大値
	};

	enum class UPDATE_TYPE
	{
		SELECT,
		APPLY,
	};
	
	//状態の最大値
	static constexpr int STATE_MAX = static_cast<int>(STATE::MAX);	
	
	//クリアスコアの最大値
	static constexpr int CLEAR_SCORE_MAX = 15;	

	//クリアスコアの最小値
	static constexpr int CLEAR_SCORE_MIN = 1;

	//クリアスコアの範囲
	static constexpr int CLEAR_SCORE_RANGE = CLEAR_SCORE_MAX - CLEAR_SCORE_MIN + 1;

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
	int imgTitle_;			//タイトル画像
	int imgTriangle_;		//三角画像
	int imgSettingFinish_;	//設定完了画像
	int* imgCursor_;		//カーソル画像
	int* imgSelectMess_;	//メッセージ
	int* imgSettings_;		//設定項目
	int* imgNumbers_;		//ナンバー画像
	int* imgOnOff_;			//オンオフ画像
	int* imgApplyMes_;		//適用時のメッセージ画像

	//ステートインデックス
	int stateIndex_;

	//クリアスコア
	int clearScore_;	

	//サウンドボリューム
	int soundVolume_[SoundManager::TYPE_MAX];
	
	//スキップ設定
	bool isSkip_;

	//フルスクリーン設定
	bool isFullScreen_;

	//更新状態
	UPDATE_TYPE updateType_;
	
	//状態別右の処理の登録
	std::unordered_map<STATE, std::function<void()>> rightStateMap_; 

	//状態別左の処理の登録
	std::unordered_map<STATE, std::function<void()>> leftStateMap_; 

	//描画処理の登録
	std::unordered_map<STATE, std::function<void()>> drawStateMap_; 

	//種類別の更新関数
	std::unordered_map<UPDATE_TYPE, std::function<void(SelectScene&)>> updateMap_;

	//更新処理
	void UpdateSelect(SelectScene& _parent);	//選択
	void UpdateApply(SelectScene& _parent);		//適用

	//データを反映
	void ApplyData();	
	
	//タイトルの描画
	void DrawTitle();

	//設定完了のメッセージの描画
	void DrawSettingFinish();

	//矢印の描画
	void DrawCursor();

	//三角の描画
	void DrawSelectTriangle();

	//項目名の描画
	void DrawSelectStateName(const int _index);

	//状態別の描画
	void DrawClearScore();
	void DrawSwitch(const STATE _state, const bool _flag);
	void DrawSoundVolume(const STATE _state, const int _type);

	//適用時のメッセージ描画
	void DrawApplyMessage();
};

