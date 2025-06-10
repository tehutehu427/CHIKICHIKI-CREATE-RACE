#include "EditorUi.h"
#include <string>
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Common/FontRegistry.h"
#include "../../Manager/System/ResourceManager.h"

//描くUI
//カメラ操作説明
//入出力
//プレイへの切り替え


namespace
{
	Vector2 screenSize = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };	//スクリーンサイズ
}

EditorUi::EditorUi()
{
	explanFont_ = -1;
	ioFont_ = -1;
}

EditorUi::~EditorUi()
{
	DeleteFontToHandle(explanFont_);
	DeleteFontToHandle(ioFont_);
}

void EditorUi::Load()
{
	//フォント生成
	explanFont_ = CreateFontToHandle(
		FontRegistry::LOGO.c_str(), 
		EXPLAN_FONT_SIZE,
		EXPLAN_FONT_THICK
		);

	ioFont_ = CreateFontToHandle(
		FontRegistry::LOGO.c_str(),
		IO_FONT_SIZE,
		IO_FONT_THICK
	);
}

void EditorUi::Init()
{
}

void EditorUi::Draw()
{
	//操作説明
	DrawEditingExplan();

	//データの入出力用アイコン
	DrawMapDataIOIcon();

	//プレイへ切り替え
	DrawChangePlay();
}

void EditorUi::DrawEditingExplan()
{
	constexpr int EXPLANS = 5;
	std::string explanString[] = {
		"前後 : ホイール",
		"左右 : A D",
		"上下回転 : W S",
		"左右回転 : Q E",
		"配置/決定 : 左クリック/ENTER",
	};

	Vector2 pos = {
		screenSize.x - 200,
		10,
	};

	//背景
	DrawBoxAA(
		pos.x, 
		pos.y,
		pos.x + 200,
		pos.y + EXPLAN_FONT_SIZE * EXPLANS,
		Utility::WHITE,
		true);

	DrawBoxAA(
		pos.x,
		pos.y,
		pos.x + 200,
		pos.y + EXPLAN_FONT_SIZE * EXPLANS,
		Utility::PINK,
		false,
		5.0f);

	for (int i = 0; i < EXPLANS; i++)
	{
		DrawFormatStringToHandle(
			pos.x,
			pos.y + i * EXPLAN_FONT_SIZE,
			Utility::RED,
			explanFont_,
			explanString[i].c_str()
		);
	}
}

void EditorUi::DrawMapDataIOIcon()
{
	constexpr int MESS = 2;
	std::string mesString[] = {
		"データ保存 : B",
		"インポート : N",
	};

	Vector2 pos = {
		0,
		16,
	};



	for (int i = 0; i < MESS; i++)
	{	
		//背景
		DrawBoxAA(
			pos.x,
			pos.y + IO_FONT_SIZE * i,
			pos.x + IO_FONT_SIZE * 6,
			pos.y + IO_FONT_SIZE * (i + 1),
			Utility::WHITE,
			true);

		DrawBoxAA(
			pos.x,
			pos.y + IO_FONT_SIZE * i,
			pos.x + IO_FONT_SIZE * 6,
			pos.y + IO_FONT_SIZE * (i + 1),
			Utility::CYAN,
			false,
			5.0f);

		DrawFormatStringToHandle(
			pos.x,
			pos.y + i * IO_FONT_SIZE,
			Utility::BLUE,
			ioFont_,
			mesString[i].c_str()
		);
	}
}

void EditorUi::DrawChangePlay()
{
	std::string mes = "PLAY : Z";

	//座標位置
	Vector2 pos = {
		0,
		screenSize.y - IO_FONT_SIZE,
	};

	//背景
	DrawBoxAA(
		pos.x,
		pos.y,
		pos.x + IO_FONT_SIZE * 6,
		pos.y + IO_FONT_SIZE,
		Utility::WHITE,
		true);

	DrawBoxAA(
		pos.x,
		pos.y,
		pos.x + IO_FONT_SIZE * 6,
		pos.y + IO_FONT_SIZE,
		Utility::LIME,
		false,
		5.0f);

	DrawFormatStringToHandle(
		pos.x,
		pos.y,
		Utility::GREEN,
		ioFont_,
		mes.c_str()
	);
}
