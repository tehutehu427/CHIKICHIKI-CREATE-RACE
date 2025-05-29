#include <DxLib.h>
#include "FontRegistry.h"
#include "../Application.h"

const std::string FontRegistry::DOT = "ベストテンDOT";
const std::string FontRegistry::BOKUTATI = "ぼくたちのゴシック";
const std::string FontRegistry::HANAZOME = "はなぞめフォント";
const std::string FontRegistry::PENGS = "PENGS";
const std::string FontRegistry::LOGO = "コーポレート・ロゴ ver3 Bold";
const std::string FontRegistry::BANANA = "YDW バナナスリップplus plus";

FontRegistry::FontRegistry()
{
}

FontRegistry::~FontRegistry()
{
}

void FontRegistry::Init()
{
	//フォント登録を行う
	// フォントパス初期化
	fontPath_[static_cast<int>(FONT_TYPE::DOT)] = "FontDot.otf";
	fontPath_[static_cast<int>(FONT_TYPE::BOKUTATI)] = "bokutachi.otf";
	fontPath_[static_cast<int>(FONT_TYPE::HANAZOME)] = "はなぞめフォント.ttf";
	fontPath_[static_cast<int>(FONT_TYPE::PENGS)] = "PENGS.ttf";
	fontPath_[static_cast<int>(FONT_TYPE::LOGO)] = "Corporate-Logo-Bold-ver3.otf";
	fontPath_[static_cast<int>(FONT_TYPE::BANANA)] = "YDWbananaslipplus.otf";

	// フォントをシステムに登録
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (AddFontResourceEx(
			(Application::PATH_FONT + fontPath_[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}

void FontRegistry::Destroy()
{
	// フォント登録解除
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (RemoveFontResourceEx(
			(Application::PATH_FONT + fontPath_[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}
